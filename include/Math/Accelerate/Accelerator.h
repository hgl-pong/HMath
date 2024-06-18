#pragma once
#include <Math/Accelerate/AccelerateCommon.h>
#include <Math/Accelerate/TreeUtils.h>
namespace MathLib
{
    template <class BBox>
    class Accelerator
    {
    private:
        typedef std::vector<TreeNode<BBox>> Tree;

    public:
        Accelerator()
            :m_Type(AcceleratorType::eBVH)
        {
        };

        ~Accelerator()
        {
        }

        void SetType(AcceleratorType type)
        {
            _ASSERT(typeid(BBox) == typeid(HAABBox2D) && type != AcceleratorType::eOctTree);
            _ASSERT(typeid(BBox) == typeid(HAABBox3D) && type != AcceleratorType::eQuadTree);
            m_Type = type;
            std::vector<BBox> bBoxes;
            TreeUtils::FindOutAllBoxes(m_Tree, bBoxes);
            Build(bBoxes);
        }

        void Build(std::vector<BBox> &bBoxes)
        {
        }

    private:
        AcceleratorType m_Type;
        Tree m_Tree;
    };

    template<>
    inline void Accelerator<HAABBox2D>::Build(std::vector<HAABBox2D>& bBoxes)
    {
        switch (m_Type)
        {
        case AcceleratorType::eBVH:
            TreeUtils::Builder::BuildBVH(bBoxes, m_Tree);
            break;
        case AcceleratorType::eAABB:
            TreeUtils::Builder::BuildAABBTree(bBoxes, m_Tree);
            break;
        case AcceleratorType::eKDTree:
            TreeUtils::Builder::BuildKDTree(bBoxes, m_Tree);
            break;
        case AcceleratorType::eQuadTree:
            TreeUtils::Builder::BuildQuadTree(bBoxes, m_Tree);
            break;
        default:
            break;
        }
    }

    template<>
    inline void Accelerator<HAABBox3D>::Build(std::vector<HAABBox3D>& bBoxes)
    {
        switch (m_Type)
        {
        case AcceleratorType::eBVH:
            TreeUtils::Builder::BuildBVH(bBoxes, m_Tree);
            break;
        case AcceleratorType::eAABB:
            TreeUtils::Builder::BuildAABBTree(bBoxes, m_Tree);
            break;
        case AcceleratorType::eKDTree:
            TreeUtils::Builder::BuildKDTree(bBoxes, m_Tree);
            break;
        case AcceleratorType::eOctTree:
            TreeUtils::Builder::BuildOctTree(bBoxes, m_Tree);
            break;
        default:
            break;
        }
    }
    
    typedef Accelerator<HAABBox2D> Accelerator2D;
    typedef Accelerator<HAABBox3D> Accelerator3D;
}; // namespace MathLib
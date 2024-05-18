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
        {
            m_Type = AcceleratorType::eBVH;
        };

        ~Accelerator()
        {
        }

        void SetType(AcceleratorType type)
        {
            m_Type = type;
            std::vector<TreeNode<BBox>> bBoxes;
            TreeUtils::FindOutAllBoxes(m_Tree, bBoxes);
            Build(bBoxes);
        }

        void Build(std::vector<BBox> &bBoxes)
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
                _ASSERT(HAABBox2D::AmbientDimAtCompileTime==2);
                TreeUtils::Builder::BuildQuadTree(bBoxes, m_Tree);
                break;
            case AcceleratorType::eOctTree:
				_ASSERT(HAABBox3D::AmbientDimAtCompileTime==3);
				TreeUtils::Builder::BuildOctTree(bBoxes, m_Tree);
				break;
            default:
                break;
            }
        }

    private:
        AcceleratorType m_Type;
        Tree m_Tree;
    };
}; // namespace MathLib
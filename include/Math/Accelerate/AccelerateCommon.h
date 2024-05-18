#pragma once
#include <Math/Math.h>

namespace MathLib
{
    template <class BBox>
    struct TreeNode
    {
        TreeNode() : m_End(UINT_MAX) {}
        TreeNode(const BBox &bbox, uint32_t index) : m_bbox(bbox), m_Index(index), m_End(UINT_MAX) {}
        TreeNode(const BBox &bbox, uint32_t index, uint32_t end) : m_bbox(bbox), m_Index(index), m_End(end) {}
        bool IsLeaf() const
        {
            return m_End != UINT_MAX;
        }
        BBox m_bbox;
        uint32_t m_Index;
        uint32_t m_End;
    };

    typedef std::vector<TreeNode<HAABBox2D>> BVHTree2D;
    typedef std::vector<TreeNode<HAABBox3D>> BVHTree3D;

    typedef std::vector<TreeNode<HAABBox2D>> AABBTree2D;
    typedef std::vector<TreeNode<HAABBox3D>> AABBTree3D;

    typedef std::vector<TreeNode<HAABBox2D>> KDTree2D;
    typedef std::vector<TreeNode<HAABBox3D>> KDTree3D;

    typedef std::vector<TreeNode<HAABBox2D>> QuadTree2D;

    typedef std::vector<TreeNode<HAABBox3D>> OctTree3D;

    enum class AcceleratorType
    {
        eBVH,
        eAABB,
        eKDTree,
        eQuadTree,
        eOctTree
    };

    template <class BBox>
    inline BBox MergeBoxes(const std::vector<BBox> &bBoxes, const size_t start = 0, size_t end = UINT_MAX)
    {
        if (end == UINT_MAX)
            end = bBoxes.size();
        BBox newBox;
        newBox.setEmpty();
        for (size_t i = start; i < end; i++)
        {
            newBox.extend(bBoxes[i]);
        }
        return newBox;
    }



}; // namespace MathLib
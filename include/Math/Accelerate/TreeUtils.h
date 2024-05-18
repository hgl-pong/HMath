#pragma once
#include <Math/Accelerate/AccelerateCommon.h>
#include <Math/MathUtils.h>
#include <stack>
#include <vector>
namespace MathLib
{
	namespace TreeUtils
	{
		namespace Builder
		{
			const int MAX_DEPTH = 10; // �������������
			const int MIN_SIZE = 4;   // �ڵ��ڷָ�ǰ���ܰ�������СԪ������

			template<class BBox>
			bool IsLeft(const BBox& bBox, const uint32_t axis, const HReal& pivot)
			{
				if (axis != 0 && axis != 1 && axis != 2)
					return false;
				return bBox.min()[axis] + bBox.max()[axis] < pivot * 2.0f;
			};

			template<class BBox>
			uint32_t Partition(std::vector<BBox>& bBoxes, const uint32_t begin, const uint32_t end, const uint32_t axis, const HReal pivot)
			{
				uint32_t left = begin;
				uint32_t right = end - 1;
				while (left != right)
				{
					if (!IsLeft(bBoxes[left], axis, pivot))
					{
						std::swap(bBoxes[left], bBoxes[right]);
						right--;
					}
					else
					{
						++left;
					}
				}
				return IsLeft(bBoxes[left], axis, pivot) ? ++left : left;
			};

			template<class BBox>
			void PreSplit(BBox& newBox, const BBox& bbox, const BBox& kDBox)
			{
				HVector3I tests(8, 8, 8);
				newBox = kDBox;
				while (tests[0] && tests[1] && tests[2])
				{
					HVectorX newBoxMin = newBox.min();
					HVectorX newBoxMax = newBox.max();
					HVectorX edge = newBoxMax - newBoxMin;
					uint32_t dimNum = static_cast<uint32_t>(edge.rows());
					const uint32_t splitDim = uint32_t(std::max_element(&edge[0], &edge[0] + dimNum) - &edge[0]);
					const HReal splitPlane = (newBoxMax[splitDim] + newBoxMin[splitDim]) * 0.5f;
					tests[splitDim]--;
					if (splitPlane < bbox.min()[splitDim])
					{
						newBoxMin[splitDim] = splitPlane;
						continue;
					}
					else if (splitPlane > bbox.max()[splitDim])
					{
						newBoxMax[splitDim] = splitPlane;
						break;
					}
					else
						break;
				}
			};

			template <class BBox>
			inline void BuildBVH(std::vector<BBox> &bBoxes, std::vector<TreeNode<BBox>> &nodes)
			{
				struct StackNode
				{
					StackNode(uint32_t nodeIndex, uint32_t begin, uint32_t end, uint32_t depth, const BBox &bbox) : mNodeIndex(nodeIndex), mBegin(begin), mEnd(end), mDepth(depth), mKdBBox(bbox) {}
					unsigned int mNodeIndex;
					unsigned int mBegin;
					unsigned int mEnd;
					unsigned int mDepth;
					BBox mKdBBox;
				};

				std::stack<StackNode> nodesStack;
				const unsigned int maxLeafSize = 1u;
				const unsigned int bBoxesCount = static_cast<unsigned int>(bBoxes.size());
				{
					BBox newBox;
					newBox = MergeBoxes(bBoxes, 0, bBoxesCount);
					nodesStack.push(StackNode(0u, 0u, bBoxesCount, 0u, newBox));
				}
				nodes.resize(1u);
				while (!nodesStack.empty())
				{
					StackNode &node = nodesStack.top();
					BBox nodeBox;
					nodeBox = MergeBoxes(bBoxes, node.mBegin, node.mEnd);
					if (node.mEnd - node.mBegin <= maxLeafSize)
					{
						nodes[node.mNodeIndex] = TreeNode<BBox>(nodeBox, node.mBegin, node.mEnd);
						nodesStack.pop();
						continue;
					}
					else
					{
						BBox kDBox;
						PreSplit(kDBox, nodeBox, node.mKdBBox);
						HVectorX kDBoxMin = kDBox.min();
						HVectorX kDBoxMax = kDBox.max();
						HVectorX nodeBoxMin = nodeBox.min();
						HVectorX nodeBoxMax = nodeBox.max();
						HVectorX edge = kDBoxMax - kDBoxMin;
						uint32_t dimNum = static_cast<uint32_t>(edge.rows());
						uint32_t splitDim = uint32_t(std::max_element(&edge[0], &edge[0] + dimNum) - &edge[0]);
						HReal splitPlane = (kDBoxMax[splitDim] + kDBoxMin[splitDim]) * 0.5f;
						uint32_t splitIndex = Partition(bBoxes,node.mBegin, node.mEnd, splitDim, splitPlane);

						if (splitIndex == node.mBegin || splitIndex == node.mEnd)
						{
							edge = nodeBoxMax - nodeBoxMin;
							splitDim = uint32_t(std::max_element(&edge[0], &edge[0] + dimNum) - &edge[0]);
							splitPlane = (nodeBoxMax[splitDim] + nodeBoxMin[splitDim]) * 0.5f;
							splitIndex = Partition(bBoxes, node.mBegin, node.mEnd, splitDim, splitPlane);
						}

						if (splitIndex == node.mBegin || splitIndex == node.mEnd)
						{
							edge = nodeBoxMax - nodeBoxMin;
							splitDim = uint32_t(std::max_element(&edge[0], &edge[0] + dimNum) - &edge[0]);
							HReal mean = 0.0f;
							for (uint32_t i = node.mBegin; i < node.mEnd; i++)
							{
								mean += bBoxes[i].min()[splitDim] + bBoxes[i].max()[splitDim];
							}
							splitPlane = mean / HReal(node.mEnd - node.mBegin);
							splitIndex = Partition(bBoxes, node.mBegin, node.mEnd, splitDim, splitPlane);
						}

						if (splitIndex == node.mBegin || splitIndex == node.mEnd)
						{
							nodes[node.mNodeIndex] = TreeNode<BBox>(nodeBox, node.mBegin, node.mEnd);
#ifndef NDEBUG
							if (node.mEnd - node.mBegin > 1000)
							{
								printf("Warning: BVH node has %d elements\n", node.mEnd - node.mBegin);
							}
#endif
							nodesStack.pop();
							continue;
						}

						const uint32_t childIndex = uint32_t(nodes.size());
						nodes.resize(childIndex + 2u);
						nodes[node.mNodeIndex] = TreeNode<BBox>(nodeBox, childIndex);
						BBox &rightBox = node.mKdBBox;
						rightBox.min()[splitDim] = splitPlane;
						StackNode rightNode(childIndex + 1u, splitIndex, node.mEnd, node.mDepth + 1u, rightBox);
						nodesStack.push(rightNode);

						BBox &leftBox = node.mKdBBox;
						leftBox.max()[splitDim] = splitPlane;
						StackNode leftNode(childIndex, node.mBegin, splitIndex, node.mDepth + 1u, leftBox);
						nodesStack.push(leftNode);
					}
					nodesStack.pop();
				}
			}

			template <class BBox>
			void BuildAABBTree(std::vector<BBox>& bBoxes, std::vector<TreeNode<BBox>>& nodes) {
				if (bBoxes.empty()) return;

				struct StackNode {
					uint32_t start, end;
					int parentIndex, childIndex;
				};
				std::stack<StackNode> stack;

				nodes.reserve(bBoxes.size() * 2); // Ԥ���㹻�Ŀռ�
				stack.push({ 0, static_cast<uint32_t>(bBoxes.size()), -1, -1 });

				while (!stack.empty()) {
					auto [start, end, parentIndex, childIndex] = stack.top();
					stack.pop();

					int nodeIndex = nodes.size();
					nodes.emplace_back(); // ���һ���½ڵ�
					if (parentIndex != -1 && childIndex != -1) {
						// TODO: ������Ը���ʵ��������¸��ڵ���ӽڵ���Ϣ
					}

					// �������һ��Ҷ�ӽڵ�
					if (end - start <= 1) {
						nodes[nodeIndex] = TreeNode<BBox>(bBoxes[start], start, end);
						continue;
					}

					// �������а�Χ�еİ�Χ��
					BBox bbox = bBoxes[start];
					for (uint32_t i = start + 1; i < end; ++i) {
						bbox.extend(bBoxes[i].min());
						bbox.extend(bBoxes[i].max());
					}
					nodes[nodeIndex].m_bbox = bbox;

					// �ҵ������
					auto diagonal = bbox.max() - bbox.min();
					uint32_t splitAxis = 0;
					for (int i = 1; i < BBox::AmbientDimAtCompileTime; ++i) {
						if (diagonal[i] > diagonal[splitAxis]) splitAxis = i;
					}

					// ���������е�ָ��Χ��
					HReal split = (bbox.min()[splitAxis] + bbox.max()[splitAxis]) / 2.0f;

					// ʹ��Partition�����ָ�
					uint32_t mid = Partition(bBoxes, start, end, splitAxis, split);

					// ��������Ԫ�ض���һ�ߵ����
					if (mid == start || mid == end) {
						mid = start + (end - start) / 2;
					}

					// ����������
					stack.push({ mid, end, nodeIndex, 1 });
					// ����������
					stack.push({ start, mid, nodeIndex, 0 });
				}
			}

			template <class BBox>
			void BuildKDTree(std::vector<BBox>& bBoxes, std::vector<TreeNode<BBox>>& nodes) {
				if (bBoxes.empty()) return;

				struct StackNode {
					uint32_t start;
					uint32_t end;
					int depth;
					int parentIndex;
					bool leftChild;
				};

				std::stack<StackNode> stack;
				stack.push({ 0, static_cast<uint32_t>(bBoxes.size()), 0, -1, true });

				while (!stack.empty()) {
					auto [start, end, depth, parentIndex, leftChild] = stack.top();
					stack.pop();

					if (start >= end) continue; // Invalid range

					int nodeIndex = nodes.size();
					nodes.emplace_back(); // Add a new node

					// If this is a leaf node
					if (end - start <= 1) {
						nodes[nodeIndex] = TreeNode<BBox>(bBoxes[start], start);
						if (parentIndex != -1) {
							// Update parent's child index here if needed
						}
						continue;
					}

					// Find the dimension to split on based on depth
					uint32_t axis = depth % BBox::AmbientDimAtCompileTime;

					// Sort the boxes along the chosen axis
					std::nth_element(bBoxes.begin() + start, bBoxes.begin() + (start + end) / 2, bBoxes.begin() + end,
						[axis](const BBox& a, const BBox& b) {
							return a.center()[axis] < b.center()[axis];
						});

					uint32_t mid = start + (end - start) / 2;

					// Create the node
					BBox bbox = bBoxes[start];
					for (uint32_t i = start + 1; i < end; ++i) {
						bbox.extend(bBoxes[i].min());
						bbox.extend(bBoxes[i].max());
					}
					nodes[nodeIndex] = TreeNode<BBox>(bbox, mid);

					// Push children to stack
					stack.push({ mid + 1, end, depth + 1, nodeIndex, false });
					stack.push({ start, mid, depth + 1, nodeIndex, true });
				}
			}

			template <class BBox>
			void BuildQuadTree(std::vector<BBox>& bBoxes, std::vector<TreeNode<BBox>>& nodes) {
				//static_assert(BBox::AmbientDimAtCompileTime == 2);
				if (bBoxes.empty()) return;

				struct StackNode {
					BBox bounds;
					uint32_t start, end;
					int depth;
				};

				std::stack<StackNode> stack;

				// �������а�Χ�еĺϲ���Χ�У���Ϊ���ڵ�İ�Χ��
				BBox rootBounds = bBoxes[0];
				for (const auto& box : bBoxes) {
					rootBounds.extend(box.min());
					rootBounds.extend(box.max());
				}

				// ��ʼ��ջ
				stack.push({ rootBounds, 0, static_cast<uint32_t>(bBoxes.size()), 0 });

				while (!stack.empty()) {
					auto [bounds, start, end, depth] = stack.top();
					stack.pop();

					int nodeIndex = nodes.size();
					nodes.emplace_back(bounds, start); // �����½ڵ�

					// ������Ȼ�����������ֹ�ָ�
					if (depth > MAX_DEPTH || (end - start) <= MIN_SIZE) {
						continue;
					}

					// �ָ��Χ��Ϊ�ĸ�����
					Eigen::Vector2f center = bounds.center();
					std::array<BBox, 4> quadrants = {
						BBox(bounds.min(), center), // ����
						BBox(Eigen::Vector2f(center[0], bounds.min()[1]), Eigen::Vector2f(bounds.max()[0], center[1])), // ����
						BBox(center, bounds.max()), // ����
						BBox(Eigen::Vector2f(bounds.min()[0], center[1]), Eigen::Vector2f(center[0], bounds.max()[1])) // ����
					};

					// Ϊÿ�����޴����ӽڵ�
					for (int i = 0; i < 4; ++i) {
						stack.push({ quadrants[i], start, end, depth + 1 });
					}
				}
			}

			template <class BBox>
			void BuildOctTree(std::vector<BBox>& bBoxes, std::vector<TreeNode<BBox>>& nodes) {
				//static_assert(BBox::AmbientDimAtCompileTime == 3);
				if (bBoxes.empty()) return;

				struct StackNode {
					HAABBox3D bounds;
					uint32_t start, end;
					int depth;
				};

				std::stack<StackNode> stack;

				// �������а�Χ�еĺϲ���Χ�У���Ϊ���ڵ�İ�Χ��
				BBox rootBounds = bBoxes[0];
				for (const auto& box : bBoxes) {
					rootBounds.extend(box.min());
					rootBounds.extend(box.max());
				}

				// ��ʼ��ջ
				stack.push({ rootBounds, 0, static_cast<uint32_t>(bBoxes.size()), 0 });

				while (!stack.empty()) {
					auto [bounds, start, end, depth] = stack.top();
					stack.pop();

					int nodeIndex = nodes.size();
					nodes.emplace_back(bounds, start); // �����½ڵ�

					// ������Ȼ�����������ֹ�ָ�
					if (depth > MAX_DEPTH || (end - start) <= MIN_SIZE) {
						continue;
					}

					// �ָ��Χ��Ϊ�˸�����
					Eigen::Vector3f center = bounds.center();
					std::array<BBox, 8> octants{
						BBox(bounds.min(), center), // ������
						BBox(Eigen::Vector3f(center[0], bounds.min()[1], bounds.min()[2]), Eigen::Vector3f(bounds.max()[0], center[1], center[2])), // ������
						BBox(Eigen::Vector3f(bounds.min()[0], center[1], bounds.min()[2]), Eigen::Vector3f(center[0], bounds.max()[1], center[2])), // ������
						BBox(Eigen::Vector3f(center[0], center[1], bounds.min()[2]), Eigen::Vector3f(bounds.max()[0], bounds.max()[1], center[2])), // ������
						BBox(Eigen::Vector3f(bounds.min()[0], bounds.min()[1], center[2]), center), // ǰ����
						BBox(Eigen::Vector3f(center[0], bounds.min()[1], center[2]), Eigen::Vector3f(bounds.max()[0], center[1], bounds.max()[2])), // ǰ����
						BBox(Eigen::Vector3f(bounds.min()[0], center[1], center[2]), Eigen::Vector3f(center[0], bounds.max()[1], bounds.max()[2])), // ǰ����
						BBox(center, bounds.max()) // ǰ����
					};

					// Ϊÿ�����޴����ӽڵ�
					for (int i = 0; i < 8; ++i) {
						stack.push({ octants[i], start, end, depth + 1 });
					}
				}
			}
		} // namespace Builder

		template <class BBox>
		inline void FindOutAllBoxes(const std::vector<TreeNode<BBox>> &nodes, std::vector<BBox> &bBoxes)
		{
			std::stack<uint32_t> nodesStack;
			nodesStack.push(0u);
			while (!nodesStack.empty())
			{
				uint32_t nodeIndex = nodesStack.top();
				nodesStack.pop();
				const TreeNode<BBox> &node = nodes[nodeIndex];
				if (node.IsLeaf())
				{
					for (uint32_t i = node.m_Index; i < node.m_End; i++)
					{
						bBoxes.push_back(node.m_bbox);
					}
				}
				else
				{
					nodesStack.push(node.m_Index);
					nodesStack.push(node.m_Index + 1);
				}
			}
		}

		template <class BBox>
		inline bool Intersect(const std::vector<TreeNode<BBox>> &nodes, const BBox &box)
		{
			std::stack<uint32_t> nodesStack;
			nodesStack.push(0u);
			while (!nodesStack.empty())
			{
				uint32_t nodeIndex = nodesStack.top();
				nodesStack.pop();
				const TreeNode<BBox> &node = nodes[nodeIndex];
				if (node.IsLeaf())
				{
					for (uint32_t i = node.m_Index; i < node.m_End; i++)
					{
						if (node.m_bbox.intersect(box))
							return true;
					}
				}
				else
				{
					nodesStack.push(node.m_Index);
					nodesStack.push(node.m_Index + 1);
				}
			}
			return false;
		}

	}; // namespace TreeUtils
} // namespace
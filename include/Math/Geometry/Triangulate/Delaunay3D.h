#pragma once
#include <Math/Math.h>
#include <Math/GraphicUtils/MeshData.h>
#include <Math/HashGrid.h>
namespace MathLib
{
	namespace Geometry
	{
		class Delaunay3D
		{
		private:
			struct TetrahedronIndex
			{
				uint32_t  vertices[4];
				uint32_t neighbors[4];
				TetrahedronIndex() {}
				TetrahedronIndex(uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3) {
					vertices[0] = v0;
					vertices[1] = v1;
					vertices[2] = v2;
					vertices[3] = v3;
				}
				bool operator==(const TetrahedronIndex& rhs) const {
					return vertices[0] == rhs.vertices[0] && vertices[1] == rhs.vertices[1] && vertices[2] == rhs.vertices[2] && vertices[3] == rhs.vertices[3];
				}
			};
			struct EdgeIndex
			{
				uint32_t vertices[2];
				EdgeIndex() {}
				EdgeIndex(uint32_t v0, uint32_t v1) {
					vertices[0] = v0;
					vertices[1] = v1;
				}
			};
			struct EdgeHash
			{
				size_t operator()(const EdgeIndex& edge) const
				{
					return std::hash<uint32_t>()(edge.vertices[0]) ^ std::hash<uint32_t>()(edge.vertices[1]);
				}
			};
			struct EdgeEqual
			{
				bool operator()(const EdgeIndex& lhs, const EdgeIndex& rhs) const
				{
					return lhs.vertices[0] == rhs.vertices[0] && lhs.vertices[1] == rhs.vertices[1];
				}
			};
		public:
			Delaunay3D() {}
			~Delaunay3D() {}

			void AddPoint(const HVector3& point) {
				_InsertPoint(point);
			}

			void SetPoints(const std::vector<HVector3>& points) {

				for (int i = 0; i < points.size(); ++i) {
					_InsertPoint(points[i]);
				}
			}

			void Clear() {
				m_Points.clear();
				m_Tetrahedrons.clear();
			}

			uint32_t GetTetrahedronCount() const {
				return m_Tetrahedrons.size();
			}

			bool GetMeshData(GraphicUtils::MeshData& meshdata)
			{
				std::vector<HVector3>& vertices = meshdata.m_Vertices;
				std::vector<uint32_t>& indices = meshdata.m_Indices;

				vertices = m_Points;

				for (const auto& tetra : m_Tetrahedrons) {
					uint32_t faceIndices[4][3] = {
						{ tetra.vertices[0], tetra.vertices[1], tetra.vertices[2] },
						{ tetra.vertices[0], tetra.vertices[1], tetra.vertices[3] },
						{ tetra.vertices[0], tetra.vertices[2], tetra.vertices[3] },
						{ tetra.vertices[1], tetra.vertices[2], tetra.vertices[3] }
					};

					for (int i = 0; i < 4; ++i) {
						indices.push_back(faceIndices[i][0]);
						indices.push_back(faceIndices[i][1]);
						indices.push_back(faceIndices[i][2]);
					}
				}
				return true;
			}
		private:
			bool _IsPointInCircumsphere(const HVector3& p, const TetrahedronIndex& tetra) {
				Eigen::Matrix4d mat;
				for (int i = 0; i < 4; ++i) {
					const HVector3& v = m_Points[tetra.vertices[i]];
					mat(i, 0) = v[0];
					mat(i, 1) = v[1];
					mat(i, 2) = v[2];
					mat(i, 3) = 1.0;
				}
				mat(3, 0) = p[0];
				mat(3, 1) = p[1];
				mat(3, 2) = p[2];
				mat(3, 3) = 1.0;

				double det = mat.determinant();
				return det > 0;
			}

			void _InsertPoint(const HVector3& point) {
				std::vector<TetrahedronIndex> badTetrahedrons;
				_FindBadTetrahedrons(badTetrahedrons);

				std::unordered_set<EdgeIndex, EdgeHash,EdgeEqual> edges;
				for (const auto& tetra : badTetrahedrons) {
					for (int i = 0; i < 4; ++i) {
						EdgeIndex edge;
						edge.vertices[0] = tetra.vertices[i];
						edge.vertices[1] = tetra.vertices[(i + 1) % 4];
						if (edge.vertices[0] > edge.vertices[1]) {
							std::swap(edge.vertices[0], edge.vertices[1]);
						}
						edges.insert(edge);
					}
				}

				std::vector<TetrahedronIndex> newTetrahedrons;
				for (const auto& tetra : badTetrahedrons) {
					for (int i = 0; i < 4; ++i) {
						EdgeIndex edge;
						edge.vertices[0] = tetra.vertices[i];
						edge.vertices[1] = tetra.vertices[(i + 1) % 4];
						if (edge.vertices[0] > edge.vertices[1]) {
							std::swap(edge.vertices[0], edge.vertices[1]);
						}
						if (edges.count(edge) > 1) {
							TetrahedronIndex newTetra;
							newTetra.vertices[0] = edge.vertices[0];
							newTetra.vertices[1] = edge.vertices[1];
							newTetra.vertices[2] = tetra.vertices[(i + 2) % 4];
							newTetra.vertices[3] = tetra.vertices[(i + 3) % 4];
							newTetrahedrons.push_back(newTetra);

							newTetra.vertices[0] = edge.vertices[1];
							newTetra.vertices[1] = edge.vertices[0];
							newTetra.vertices[2] = tetra.vertices[(i + 2) % 4];
							newTetra.vertices[3] = tetra.vertices[(i + 3) % 4];
							newTetrahedrons.push_back(newTetra);
						}
					}
				}
			}

			void _RemovePoint(uint32_t& pointIndex)
			{}

			void _FindBadTetrahedrons(std::vector<TetrahedronIndex>& badTetrahedrons) {
				std::unordered_set<EdgeIndex, EdgeHash,EdgeEqual> edges;
				for (const auto& tetra : m_Tetrahedrons) {
					for (int i = 0; i < 4; ++i) {
						EdgeIndex edge;
						edge.vertices[0] = tetra.vertices[i];
						edge.vertices[1] = tetra.vertices[(i + 1) % 4];
						if (edge.vertices[0] > edge.vertices[1]) {
							std::swap(edge.vertices[0], edge.vertices[1]);
						}
						edges.insert(edge);
					}
				}

				for (const auto& tetra : m_Tetrahedrons) {
					for (int i = 0; i < 4; ++i) {
						EdgeIndex edge;
						edge.vertices[0] = tetra.vertices[i];
						edge.vertices[1] = tetra.vertices[(i + 1) % 4];
						if (edge.vertices[0] > edge.vertices[1]) {
							std::swap(edge.vertices[0], edge.vertices[1]);
						}
						if (edges.count(edge) > 1) {
							badTetrahedrons.push_back(tetra);
							break;
						}
					}
				}
			}

			void _FlipEdges(std::vector<TetrahedronIndex>& tetrahedra, HashGrid3D<TetrahedronIndex>& hashGrid) {
				std::unordered_set<EdgeIndex, EdgeHash,EdgeEqual> edgesToCheck;
				for (const auto& tetra : tetrahedra) {
					edgesToCheck.insert(EdgeIndex(tetra.vertices[0], tetra.vertices[1]));
					edgesToCheck.insert(EdgeIndex(tetra.vertices[0], tetra.vertices[2]));
					edgesToCheck.insert(EdgeIndex(tetra.vertices[0], tetra.vertices[3]));
					edgesToCheck.insert(EdgeIndex(tetra.vertices[1], tetra.vertices[2]));
					edgesToCheck.insert(EdgeIndex(tetra.vertices[1], tetra.vertices[3]));
					edgesToCheck.insert(EdgeIndex(tetra.vertices[2], tetra.vertices[3]));
				}

				while (!edgesToCheck.empty()) {
					auto it = edgesToCheck.begin();
					EdgeIndex edge = *it;
					edgesToCheck.erase(it);

					// 查找共享该边的四面体
					std::vector<TetrahedronIndex*> adjacentTetrahedra;
					for (auto& tetra : tetrahedra) {
						bool hasEdge = (std::find(std::begin(tetra.vertices), std::end(tetra.vertices), edge.vertices[0]) != std::end(tetra.vertices)) &&
							(std::find(std::begin(tetra.vertices), std::end(tetra.vertices), edge.vertices[1]) != std::end(tetra.vertices));
						if (hasEdge) {
							adjacentTetrahedra.push_back(&tetra);
						}
					}

					// 如果有两个四面体共享该边，则检查并翻转
					if (adjacentTetrahedra.size() == 2) {
						TetrahedronIndex* t1 = adjacentTetrahedra[0];
						TetrahedronIndex* t2 = adjacentTetrahedra[1];

						// 找到两个四面体中不在该边上的点
						uint32_t p1 = -1;
						uint32_t p2 = -1;
						for (auto p : t1->vertices) {
							if (p != edge.vertices[0] && p != edge.vertices[1]) {
								p1 = p;
								break;
							}
						}
						for (auto p : t2->vertices) {
							if (p != edge.vertices[0] && p != edge.vertices[1]) {
								p2 = p;
								break;
							}
						}

						if (p1 && p2 && _IsPointInCircumsphere(m_Points[p2], *t1)) {
							// 翻转边
							TetrahedronIndex newTetra1(p1, p2, edge.vertices[0], edge.vertices[1]);
							TetrahedronIndex newTetra2(p1, p2, edge.vertices[1], edge.vertices[0]);

							// 更新四面体列表和哈希网格
							tetrahedra.erase(std::remove(tetrahedra.begin(), tetrahedra.end(), *t1), tetrahedra.end());
							tetrahedra.erase(std::remove(tetrahedra.begin(), tetrahedra.end(), *t2), tetrahedra.end());
							hashGrid.DeletePoint(m_Points[t1->vertices[0]], *t1);
							hashGrid.DeletePoint(m_Points[t2->vertices[0]], *t2);

							tetrahedra.push_back(newTetra1);
							tetrahedra.push_back(newTetra2);
							hashGrid.AddPoint(m_Points[newTetra1.vertices[0]], newTetra1);
							hashGrid.AddPoint(m_Points[newTetra2.vertices[0]], newTetra2);

							// 将新的边加入检查列表
							edgesToCheck.insert(EdgeIndex(p1, p2));
							edgesToCheck.insert(EdgeIndex(p1, edge.vertices[0]));
							edgesToCheck.insert(EdgeIndex(p1, edge.vertices[1]));
							edgesToCheck.insert(EdgeIndex(p2, edge.vertices[0]));
							edgesToCheck.insert(EdgeIndex(p2, edge.vertices[1]));
						}
					}
				}
			}

		private:
			std::vector<HVector3> m_Points;
			std::vector<TetrahedronIndex> m_Tetrahedrons;
		};
	}; // namespace Geometry
}; // namespace MathLib
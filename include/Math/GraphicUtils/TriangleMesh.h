#pragma once
#include <Math/Math.h>
#include <Math/GraphicUtils/MeshCommon.h>
#include <vector>
namespace MathLib
{
	namespace MeshTool
	{
		template <typename IntType, typename = std::enable_if_t<std::is_integral<IntType>::value>>
		class TriangleMesh
		{
		public:
			TriangleMesh() = delete;
			TriangleMesh(const std::vector<HVector3> &vertices, const std::vector<HVector3> &normals, const std::vector<std::vector<HVector2>> &texCoords, const std::vector<TriangleIndex<IntType>> &triangles)
				: m_Vertices(vertices), m_Normals(normals), m_TexCoords(texCoords), m_Triangles(triangles)
			{
				UpdateBoundingBox();
			}

			TriangleMesh(const std::vector<HVector3> &vertices, const std::vector<std::vector<HVector2>> &texCoords, const std::vector<TriangleIndex<IntType>> &triangles)
				: m_Vertices(vertices), m_TexCoords(texCoords), m_Triangles(triangles)
			{
				ComputeNormals();
				UpdateBoundingBox();
			}

			TriangleMesh(const std::vector<HVector3> &vertices, const std::vector<TriangleIndex<IntType>> &triangles)
				: m_Vertices(vertices), m_Triangles(triangles)
			{
				ComputeNormals();
				UpdateBoundingBox();
			}

			TriangleMesh(const std::vector<HVector3> &vertices, const std::vector<IntType> &triangles)
				: m_Vertices(vertices)
			{
				if (triangles.size() % 3 != 0)
					return;
				const uint32_t triangleCount = triangles.size() / 3;
				m_Triangles.resize(triangleCount);
				for (uint32_t i = 0; i < triangleCount; i++)
				{
					m_Triangles[i] = TriangleIndex(triangles[3 * i], triangles[3 * i + 1], triangles[3 * i + 2]);
				}
				ComputeNormals();
				UpdateBoundingBox();
			}

			TriangleMesh(const TriangleMesh &other)
				: m_Vertices(other.m_Vertices), m_Normals(other.m_Normals), m_TexCoords(other.m_TexCoords), m_Triangles(other.m_Triangles)
			{
				UpdateBoundingBox();
			}

			TriangleMesh &operator=(const TriangleMesh &other)
			{
				m_Vertices = other.m_Vertices;
				m_Normals = other.m_Normals;
				m_TexCoords = other.m_TexCoords;
				m_Triangles = other.m_Triangles;
				m_BoundingBox = other.m_BoundingBox;
				m_BoundingBoxes = other.m_BoundingBoxes;
				return *this;
			}

			bool IsValid() const
			{
				return m_Vertices.size() > 0 && m_Normals.size() > 0 && m_TexCoords.size() > 0 && m_Triangles.size() > 0;
			}

			const std::vector<HVector3> &GetVertices() const
			{
				return m_Vertices;
			}

			const std::vector<HVector3> &GetNormals() const
			{
				return m_Normals;
			}

			const std::vector<std::vector<HVector2>> &GetTexCoords() const
			{
				return m_TexCoords;
			}

			const std::vector<TriangleIndex<IntType>> &GetTriangles() const
			{
				return m_Triangles;
			}

			const size_t GetVertexCount() const
			{
				return m_Vertices.size();
			}

			const std::vector<HAABBox3D> &GetBoundingBoxes()
			{
				if (m_BoundingBoxes.empty())
					UpdateBoundingBoxes();
				return m_BoundingBoxes;
			}

			const HAABBox3D &GetBoundingBox() const
			{
				return m_BoundingBox;
			}

			const size_t GetTriangleCount() const
			{
				return m_Triangles.size();
			}

			void ComputeNormals()
			{
				m_Normals.resize(m_Vertices.size(), HVector3(0.0f, 0.0f, 0.0f));

				for (const TriangleIndex<IntType> &triangle : m_Triangles)
				{
					HVector3 normal = (m_Vertices[triangle.vertices[1]] - m_Vertices[triangle.vertices[0]]).cross(m_Vertices[triangle.vertices[2]] - m_Vertices[triangle.vertices[0]]).normalized();

					for (int i = 0; i < 3; i++)
					{
						m_Normals[triangle.vertices[i]] += normal;
					}
				}

				for (HVector3 &normal : m_Normals)
				{
					normal.normalize();
				}
			}

			void UpdateBoundingBoxes()
			{
				m_BoundingBoxes.resize(m_Triangles.size());

				for (size_t i = 0; i < m_Triangles.size(); i++)
				{
					const TriangleIndex<IntType> &triangle = m_Triangles[i];
					m_BoundingBoxes[i].setEmpty();
					m_BoundingBoxes[i].extend(m_Vertices[triangle.vertices[0]]);
					m_BoundingBoxes[i].extend(m_Vertices[triangle.vertices[1]]);
					m_BoundingBoxes[i].extend(m_Vertices[triangle.vertices[2]]);
				}
				UpdateBoundingBox();
			}

			void UpdateBoundingBox()
			{
				m_BoundingBox.setEmpty();

				for (const HVector3 &vertex : m_Vertices)
				{
					m_BoundingBox.extend(vertex);
				}
			}

		private:
			HAABBox3D m_BoundingBox;
			std::vector<HAABBox3D> m_BoundingBoxes;
			std::vector<HVector3> m_Vertices;
			std::vector<HVector3> m_Normals;
			std::vector<std::vector<HVector2>> m_TexCoords;
			std::vector<TriangleIndex<IntType>> m_Triangles;
		};

		typedef TriangleMesh<uint8_t> TriangleMesh8;
		typedef TriangleMesh<uint16_t>TriangleMesh16;
		typedef TriangleMesh<uint32_t> TriangleMesh32;
		typedef TriangleMesh<uint64_t> TriangleMesh64;
	} // namespace MeshTool
} // namespace MathLib
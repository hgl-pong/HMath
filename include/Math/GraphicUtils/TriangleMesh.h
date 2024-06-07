#pragma once
#include <Math/Math.h>
#include <Math/Mesh/MeshCommon.h>
#include <vector>
namespace MathLib
{
	namespace MeshTool
	{
		class TriangleMesh
		{
		public:
			TriangleMesh() = delete;
			TriangleMesh(const std::vector<HVector3> &vertices, const std::vector<HVector3> &normals, const std::vector<std::vector<HVector2>> &texCoords, const std::vector<Triangle> &triangles)
				: m_Vertices(vertices), m_Normals(normals), m_TexCoords(texCoords), m_Triangles(triangles)
			{
			}

			TriangleMesh(const std::vector<HVector3> &vertices, const std::vector<std::vector<HVector2>> &texCoords, const std::vector<Triangle> &triangles)
				: m_Vertices(vertices), m_TexCoords(texCoords), m_Triangles(triangles)
			{
				ComputeNormals();
			}

			TriangleMesh(const TriangleMesh &other)
				: m_Vertices(other.m_Vertices), m_Normals(other.m_Normals), m_TexCoords(other.m_TexCoords), m_Triangles(other.m_Triangles)
			{
			}

			TriangleMesh &operator=(const TriangleMesh &other)
			{
				m_Vertices = other.m_Vertices;
				m_Normals = other.m_Normals;
				m_TexCoords = other.m_TexCoords;
				m_Triangles = other.m_Triangles;
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

			const std::vector<Triangle> &GetTriangles() const
			{
				return m_Triangles;
			}

			const size_t GetVertexCount() const
			{
				return m_Vertices.size();
			}

			const size_t GetTriangleCount() const
			{
				return m_Triangles.size();
			}

			void ComputeNormals()
			{
				m_Normals.resize(m_Vertices.size(), HVector3(0.0f, 0.0f, 0.0f));

				for (const Triangle &triangle : m_Triangles)
				{
					HVector3 normal = (m_Vertices[triangle.m_Vertices[1]] - m_Vertices[triangle.m_Vertices[0]]).cross(m_Vertices[triangle.m_Vertices[2]] - m_Vertices[triangle.m_Vertices[0]]).normalized();

					for (int i = 0; i < 3; i++)
					{
						m_Normals[triangle.m_Vertices[i]] += normal;
					}
				}

				for (HVector3 &normal : m_Normals)
				{
					normal.normalize();
				}
			}

		private:
			std::vector<HVector3> m_Vertices;
			std::vector<HVector3> m_Normals;
			std::vector<std::vector<HVector2>> m_TexCoords;
			std::vector<Triangle> m_Triangles;
		};

	} // namespace MeshTool
} // namespace MathLib
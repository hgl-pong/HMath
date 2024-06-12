#pragma once
#include <Math/Math.h>
#include <unordered_set>
namespace MathLib
{
	namespace GraphicUtils
	{
		struct MeshData
		{
			std::vector<HVector3> m_Vertices;
			std::vector<uint32_t> m_Indices;
		};

		struct pair_hash {
			template <class T1, class T2>
			std::size_t operator()(const std::pair<T1, T2>& p) const {
				auto hash1 = std::hash<T1>{}(p.first);
				auto hash2 = std::hash<T2>{}(p.second);
				return hash1 ^ (hash2 << 1); 
			}
		};

		inline std::vector<uint32_t> TrianglesToLines(const std::vector<uint32_t>& triangles) {
			std::vector<uint32_t> lines;
			std::unordered_set<std::pair<uint32_t, uint32_t>, pair_hash> edges;
			for (size_t i = 0; i < triangles.size(); i += 3) {
				int v0 = triangles[i];
				int v1 = triangles[i + 1];
				int v2 = triangles[i + 2];

				edges.insert(std::make_pair(v0, v1));
				edges.insert(std::make_pair(v1, v2));
				edges.insert(std::make_pair(v2, v0));
			}

			for (const auto& edge : edges) {
				lines.push_back(edge.first);
				lines.push_back(edge.second);
			}

			return lines;
		}

		inline MeshData GenerateGrid3DWireFrameMeshData(const HVector2I& subdivision) {
			MeshData meshData;

			int numVerticesX = subdivision[0] + 1;
			int numVerticesY = subdivision[1] + 1;

			// 生成顶点
			for (int y = 0; y < numVerticesY; ++y) {
				for (int x = 0; x < numVerticesX; ++x) {
					float fx = static_cast<float>(x) / subdivision[0] - 0.5f; // 调整x坐标使中心在原点
					float fy = static_cast<float>(y) / subdivision[1] - 0.5f; // 调整z坐标使中心在原点
					meshData.m_Vertices.emplace_back(fx, 0.0f, fy);
				}
			}

			// 生成水平线和垂直线索引
			for (int y = 0; y < numVerticesY; ++y) {
				for (int x = 0; x < subdivision[0]; ++x) {
					int startIndex = y * numVerticesX + x;
					meshData.m_Indices.push_back(startIndex);
					meshData.m_Indices.push_back(startIndex + 1);
				}
			}

			for (int x = 0; x < numVerticesX; ++x) {
				for (int y = 0; y < subdivision[1]; ++y) {
					int startIndex = y * numVerticesX + x;
					meshData.m_Indices.push_back(startIndex);
					meshData.m_Indices.push_back(startIndex + numVerticesX);
				}
			}

			return meshData;
		}

		inline MeshData GenerateSphereMeshData(HReal radius, const uint32_t slices = 10, const uint32_t stacks = 10)
		{
			MeshData meshData;

			std::vector<HVector3> &vertices = meshData.m_Vertices;
			std::vector<uint32_t> &indices = meshData.m_Indices;

			// Generate vertices
			for (uint32_t i = 0; i <= stacks; ++i)
			{
				HReal phi = H_PI * HReal(i) / HReal(stacks);
				HReal cosPhi = cos(phi);
				HReal sinPhi = sin(phi);

				for (uint32_t j = 0; j <= slices; ++j)
				{
					HReal theta = 2.0f * H_PI * HReal(j) / HReal(slices);
					HReal cosTheta = cos(theta);
					HReal sinTheta = sin(theta);

					HVector3 vertex;
					vertex[0] = radius * sinPhi * cosTheta;
					vertex[1] = radius * cosPhi;
					vertex[2] = radius * sinPhi * sinTheta;
					vertices.push_back(vertex);
				}
			}

			// Generate indices
			for (uint32_t i = 0; i < stacks; ++i)
			{
				for (uint32_t j = 0; j < slices; ++j)
				{
					uint32_t first = i * (slices + 1) + j;
					uint32_t second = first + slices + 1;

					indices.push_back(first);
					indices.push_back(second);
					indices.push_back(first + 1);

					indices.push_back(second);
					indices.push_back(second + 1);
					indices.push_back(first + 1);
				}
			}
			return meshData;
		}

		inline MeshData GenerateBoxMeshData(const HVector3& halfSize, const HVector3 center = HVector3(0,0,0))
		{
			MeshData meshData;

			std::vector<HVector3>& vertices = meshData.m_Vertices;
			std::vector<uint32_t>& indices = meshData.m_Indices;

			vertices = {
				{center[0] - halfSize[0], center[1] - halfSize[1], center[2] - halfSize[2]},
				{center[0] + halfSize[0], center[1] - halfSize[1], center[2] - halfSize[2]},
				{center[0] + halfSize[0], center[1] + halfSize[1], center[2] - halfSize[2]},
				{center[0] - halfSize[0], center[1] + halfSize[1], center[2] - halfSize[2]},
				{center[0] - halfSize[0], center[1] - halfSize[1], center[2] + halfSize[2]},
				{center[0] + halfSize[0], center[1] - halfSize[1], center[2] + halfSize[2]},
				{center[0] + halfSize[0], center[1] + halfSize[1], center[2] + halfSize[2]},
				{center[0] - halfSize[0], center[1] + halfSize[1], center[2] + halfSize[2]}
			};

			indices = {
				0, 1, 2, 0, 2, 3, // Front face
				1, 5, 6, 1, 6, 2, // Right face
				5, 4, 7, 5, 7, 6, // Back face
				4, 0, 3, 4, 3, 7, // Left face
				3, 2, 6, 3, 6, 7, // Top face
				4, 5, 1, 4, 1, 0  // Bottom face
			};
			return meshData;
		}

		inline MeshData GenerateBoxWireFrameMeshData(const HVector3& halfSize, const HVector3 center = HVector3(0, 0, 0))
		{
			MeshData meshData;

			std::vector<HVector3>& vertices = meshData.m_Vertices;
			std::vector<uint32_t>& indices = meshData.m_Indices;

			vertices = {
				{center[0] - halfSize[0], center[1] - halfSize[1], center[2] - halfSize[2]},
				{center[0] + halfSize[0], center[1] - halfSize[1], center[2] - halfSize[2]},
				{center[0] + halfSize[0], center[1] + halfSize[1], center[2] - halfSize[2]},
				{center[0] - halfSize[0], center[1] + halfSize[1], center[2] - halfSize[2]},
				{center[0] - halfSize[0], center[1] - halfSize[1], center[2] + halfSize[2]},
				{center[0] + halfSize[0], center[1] - halfSize[1], center[2] + halfSize[2]},
				{center[0] + halfSize[0], center[1] + halfSize[1], center[2] + halfSize[2]},
				{center[0] - halfSize[0], center[1] + halfSize[1], center[2] + halfSize[2]}
			};

			indices = {
				0, 1, 1,2, 2, 3,3,0, // Front face
				5, 4, 4, 7, 7, 6,6,5, // Back face
				0,4,1,5,2,6,3,7
			};
			return meshData;
		}

		inline MeshData GenerateCylinderMeshData(HReal radius, HReal height, const uint32_t slices = 10, const uint32_t stacks = 10)
		{
			MeshData meshData;

			std::vector<HVector3> &vertices = meshData.m_Vertices;
			std::vector<uint32_t> &indices = meshData.m_Indices;

			// Generate vertices
			for (uint32_t i = 0; i <= stacks; ++i)
			{
				HReal phi = H_PI * HReal(i) / HReal(stacks);
				HReal cosPhi = cos(phi);
				HReal sinPhi = sin(phi);

				for (uint32_t j = 0; j <= slices; ++j)
				{
					HReal theta = 2.0f * H_PI * HReal(j) / HReal(slices);
					HReal cosTheta = cos(theta);
					HReal sinTheta = sin(theta);

					HVector3 vertex;
					vertex[0] = radius * cosTheta;
					vertex[1] = height * 0.5f - height * HReal(i) / HReal(stacks);
					vertex[2] = radius * sinTheta;
					vertices.push_back(vertex);
				}
			}

			// Generate indices
			for (uint32_t i = 0; i < stacks; ++i)
			{
				for (uint32_t j = 0; j < slices; ++j)
				{
					uint32_t first = i * (slices + 1) + j;
					uint32_t second = first + slices + 1;

					indices.push_back(first);
					indices.push_back(second);
					indices.push_back(first + 1);

					indices.push_back(second);
					indices.push_back(second + 1);
					indices.push_back(first + 1);
				}
			}
			return meshData;
		}

		inline MeshData GenerateConeMeshData(HReal radius, HReal height, const uint32_t slices = 10)
		{
			MeshData meshData;

			std::vector<HVector3> &vertices = meshData.m_Vertices;
			std::vector<uint32_t> &indices = meshData.m_Indices;

			// Generate vertices
			HVector3 topVertex = {0.0f, height * 0.5f, 0.0f};
			vertices.push_back(topVertex);

			for (uint32_t i = 0; i <= slices; ++i)
			{
				HReal theta = 2.0f * H_PI * HReal(i) / HReal(slices);
				HReal cosTheta = cos(theta);
				HReal sinTheta = sin(theta);

				HVector3 vertex;
				vertex[0] = radius * cosTheta;
				vertex[1] = -height * 0.5f;
				vertex[2] = radius * sinTheta;
				vertices.push_back(vertex);
			}

			// Generate indices
			for (uint32_t i = 1; i <= slices; ++i)
			{
				indices.push_back(0);
				indices.push_back(i);
				indices.push_back(i + 1);
			}
			indices.push_back(0);
			indices.push_back(slices);
			indices.push_back(1);
			return meshData;
		}

		inline MeshData GeneratePyramidMeshData(HReal baseSize, HReal height)
		{
			MeshData meshData;

			std::vector<HVector3> &vertices = meshData.m_Vertices;
			std::vector<uint32_t> &indices = meshData.m_Indices;

			HVector3 topVertex = {0.0f, height * 0.5f, 0.0f};
			vertices.push_back(topVertex);

			HVector3 frontLeft = {-baseSize * 0.5f, -height * 0.5f, -baseSize * 0.5f};
			HVector3 frontRight = {baseSize * 0.5f, -height * 0.5f, -baseSize * 0.5f};
			HVector3 backRight = {baseSize * 0.5f, -height * 0.5f, baseSize * 0.5f};
			HVector3 backLeft = {-baseSize * 0.5f, -height * 0.5f, baseSize * 0.5f};

			vertices.push_back(frontLeft);
			vertices.push_back(frontRight);
			vertices.push_back(backRight);
			vertices.push_back(backLeft);

			indices = {
				0, 1, 2, // Front face
				0, 2, 3, // Right face
				0, 3, 4, // Back face
				0, 4, 1, // Left face
				1, 2, 3, // Bottom face
				1, 3, 4	 // Bottom face
			};
			return meshData;
		}

		inline void ComputeOrthoNormalBasis(const HVector3& normal, HVector3& tangent, HVector3& bitangent)
		{
			if (normal[0] != 0.0f || normal[1] != 0.0f)
			{
				tangent[0] = -normal[1];
				tangent[1] = normal[0];
				tangent[2] = 0.0f;
			}
			else
			{
				tangent[0] = 1.0f;
				tangent[1] = 0.0f;
				tangent[2] = 0.0f;
			}
			tangent = tangent.normalized();
			bitangent = normal.cross(tangent);
		}

		inline MeshData GeneratePlaneMeshData(const HVector3& normal, const HReal distance)
		{
			MeshData meshData;

			std::vector<HVector3> &vertices = meshData.m_Vertices;
			std::vector<uint32_t> &indices = meshData.m_Indices;

			HVector3 tangent, bitangent;
			ComputeOrthoNormalBasis(normal, tangent, bitangent);

			HVector3 p0 = normal * distance + tangent + bitangent;
			HVector3 p1 = normal * distance - tangent + bitangent;
			HVector3 p2 = normal * distance - tangent - bitangent;
			HVector3 p3 = normal * distance + tangent - bitangent;

			vertices = {p0, p1, p2, p3};
			indices = {0, 1, 2, 0, 2, 3};
			return meshData;
		}	

		inline MeshData GenerateCapsuleMeshData(HReal radius, HReal halfHeight, const uint32_t slices = 10, const uint32_t stacks = 10)
		{
			MeshData meshData;

			std::vector<HVector3> &vertices = meshData.m_Vertices;
			std::vector<uint32_t> &indices = meshData.m_Indices;

			// Generate vertices
			for (uint32_t i = 0; i <= stacks; ++i)
			{
				HReal phi = H_PI * HReal(i) / HReal(stacks);
				HReal cosPhi = cos(phi);
				HReal sinPhi = sin(phi);

				for (uint32_t j = 0; j <= slices; ++j)
				{
					HReal theta = 2.0f * H_PI * HReal(j) / HReal(slices);
					HReal cosTheta = cos(theta);
					HReal sinTheta = sin(theta);

					HVector3 vertex;
					vertex[0] = radius * cosPhi + (i <= stacks / 2 ? halfHeight : -halfHeight);
					vertex[1] = radius * sinPhi * cosTheta;
					vertex[2] = radius * sinPhi * sinTheta;
					vertices.push_back(vertex);
				}
			}

			// Generate indices
			for (uint32_t i = 0; i < stacks; ++i)
			{
				for (uint32_t j = 0; j < slices; ++j)
				{
					uint32_t first = i * (slices + 1) + j;
					uint32_t second = first + slices + 1;

					indices.push_back(first);
					indices.push_back(second);
					indices.push_back(first + 1);

					indices.push_back(second);
					indices.push_back(second + 1);
					indices.push_back(first + 1);
				}
			}
			return meshData;
		}
	}
}
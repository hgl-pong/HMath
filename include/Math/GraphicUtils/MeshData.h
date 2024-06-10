#pragma once
#include <Math/Math.h>
namespace MathLib
{
	namespace GraphicUtils
	{
		struct MeshData
		{
			std::vector<HVector3> m_Vertices;
			std::vector<uint32_t> m_Indices;
		};

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

		inline MeshData GenerateBoxMeshData(const HVector3 &halfSize)
		{
			MeshData meshData;

			std::vector<HVector3> &vertices = meshData.m_Vertices;
			std::vector<uint32_t> &indices = meshData.m_Indices;

			vertices = {
				{-halfSize[0], -halfSize[1], -halfSize[2]},
				{halfSize[0], -halfSize[1], -halfSize[2]},
				{halfSize[0], halfSize[1], -halfSize[2]},
				{-halfSize[0], halfSize[1], -halfSize[2]},
				{-halfSize[0], -halfSize[1], halfSize[2]},
				{halfSize[0], -halfSize[1], halfSize[2]},
				{halfSize[0], halfSize[1], halfSize[2]},
				{-halfSize[0], halfSize[1], halfSize[2]}};

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

		inline MeshData GenerateBoxWireFrameMeshData(const HVector3& halfSize)
		{
			MeshData meshData;

			std::vector<HVector3>& vertices = meshData.m_Vertices;
			std::vector<uint32_t>& indices = meshData.m_Indices;

			vertices = {
				{-halfSize[0], -halfSize[1], -halfSize[2]},
				{halfSize[0], -halfSize[1], -halfSize[2]},
				{halfSize[0], halfSize[1], -halfSize[2]},
				{-halfSize[0], halfSize[1], -halfSize[2]},
				{-halfSize[0], -halfSize[1], halfSize[2]},
				{halfSize[0], -halfSize[1], halfSize[2]},
				{halfSize[0], halfSize[1], halfSize[2]},
				{-halfSize[0], halfSize[1], halfSize[2]} };

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

		inline MeshData
		GenerateCapsuleMeshData(HReal radius, HReal halfHeight, const uint32_t slices = 10, const uint32_t stacks = 10)
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
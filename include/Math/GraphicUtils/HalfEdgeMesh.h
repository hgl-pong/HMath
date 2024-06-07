#pragma once
#include <Math/Math.h>
#include <Math/Mesh/MeshCommon.h>
#include <vector>

namespace MathLib
{
    namespace MeshTool
    {

        class HalfEdgeMesh
        {
        private:
            struct HalfEdge
            {
                int vertex;
                int face;
                int next;
                int pair;
            };

            struct Face
            {
                int edge;
            };

            struct Vertex
            {
                HVector3 position;
                int edge;
            };

        public:
        private:
        }
    } // namespace MeshTool
} // namespace MathLib

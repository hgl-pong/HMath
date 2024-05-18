#pragma once
#include <Math/Math.h>

namespace MathLib
{
    namespace PreconditionerTool
    {
        class Preconditioner
        {
        public:
            virtual void Apply(HVectorX &w, const HVectorX &v) = 0;
        };
    } // namespace PreconditionerTool
    namespace SolverTool
    {
        class LinearSolverBase
        {
        public:
            virtual int Solve(HVectorX &x, const HVectorX &b) = 0;
        };
    } // namespace SolverTool
} // namespace MathLib
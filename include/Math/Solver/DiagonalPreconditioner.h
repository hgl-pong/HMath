#pragma once
#include <Math/Math.h>
#include <Math/Solver/SolverCommon.h>

namespace MathLib
{
    namespace PreconditionerTool
    {
        class DiagonalPreconditioner : virtual public Preconditioner
        {
        public:
            DiagonalPreconditioner(HMatrixX &matrix)
            {
                for (int i = 0; i < matrix.rows(); i++)
                {
                    HReal value = matrix.coeff(i, i);
                    if (value == 0.0f)
                        m_Diagonals(i) = 1;
                    else
                        m_Diagonals(i) = HReal(1) / (value > 0 ? value : -value);
                }
            }

            void Apply(HVectorX &w, const HVectorX &v) override
            {
                assert(w.size() == m_Diagonals.size());
                assert(v.size() == m_Diagonals.size());
                for (int i = 0; i < w.size(); i++)
                    w(i) = m_Diagonals(i) * v(i);
            }

        private:
            HVectorX m_Diagonals;
        };
    } // namespace PreconditionerTool
} // namespace MathLib

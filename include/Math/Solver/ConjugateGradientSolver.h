#pragma once
#include <Math/Math.h>
#include <Math/Solver/SolverCommon.h>
#include <Math/Solver/DiagonalPreconditioner.h>
#include <memory>
namespace MathLib
{
    namespace SolverTool
    {
        class ConjugateGradientSolver : virtual public LinearSolverBase
        {
        public:
            ConjugateGradientSolver(HMatrixX &matrix)
                : m_Matrix(matrix),
                  m_MaxIterations(std::max(matrix.rows(), matrix.cols())),
                  m_Iterations(0),
                  m_Tolerance(1e-11),
                  m_Preconditioner(nullptr)
            {
            }
            ~ConjugateGradientSolver()
            {
                m_Preconditioner.reset();
            }

            void SetMaxIterations(uint32_t maxIterations) { m_MaxIterations = maxIterations; }
            void SetTolerance(HReal tolerance) { m_Tolerance = tolerance; }
            void SetPreconditioner(PreconditionerTool::Preconditioner *preconditioner) { m_Preconditioner = preconditioner; }
            uint32_t GetIterations() const { return m_Iterations; }
            uint32_t GetMaxIterations() const { return m_MaxIterations; }
            HReal GetTolerance() const { return m_Tolerance; }

            void Solve(HVectorX &x, const HVectorX &b)
            {
                m_Iterations = 0;
                m_Preconditioner = std::make_unique<PreconditionerTool::DiagonalPreconditioner>(m_Matrix);

                HVectorX r = b;
                m_Matrix.multiply(x, r, -1.0, 1.0);

                HVectorX z(m_Matrix.cols());
                m_Preconditioner->Apply(z, r);

                HReal rkDotzk = r.dot(z);
                HVectorX p = z;
                HVectorX w(m_Matrix.rows());

                while (r.norm() >= m_Tolerance && m_Iterations < m_MaxIterations)
                {
                    w.setZero();
                    m_Matrix.multiply(w, p);

                    HReal alpha = rkDotzk / p.dot(w);
                    x += alpha * p;
                    r -= alpha * w;

                    m_Preconditioner->Apply(z, r);

                    HReal rkDotzkNew = r.dot(z);
                    HReal beta = rkDotzkNew / rkDotzk;
                    p = z + beta * p;
                    rkDotzk = rkDotzkNew;
                    m_Iterations++;
                }

                m_Preconditioner.reset();
#ifdef _DEBUG
                HVectorX residual(x.size());
                residual.setZero();
                m_Matrix.multiply(residual, x);
                residual -= b;
                HReal infNorm = std::fabs(residual.maxCoeff());
                if (infNorm > 1.0e-6)
                {
                    std::cout << "ConjugateGradientSolver::Solve() - residual inf norm: " << infNorm << std::endl;
                    return -1;
                }
#endif
                return 0;
            }

        private:
            uint32_t m_MaxIterations;
            uint32_t m_Iterations;
            HReal m_Tolerance;
            HMatrixX &m_Matrix;
            std::unique_ptr<PreconditionerTool::Preconditioner> m_Preconditioner;
        };
    } // namespace SolverTool
} // namespace MathLib
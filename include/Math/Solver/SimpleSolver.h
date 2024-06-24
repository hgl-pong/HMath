#pragma once
#include <Math/Math.h>
#include <Math/Solver/SolverCommon.h>

namespace MathLib
{
    namespace SolverTool
    {
        class SimpleLinearSolver : public LinearSolverBase
        {
        public:
            SimpleLinearSolver(HMatrixX &matrix)
                : m_Matrix(matrix)
            {
            }

            int Solve(HVectorX &x, const HVectorX &b) override
            {
                const HSparseMatrixColMajor& matrix = smart_cast<HSparseMatrixColMajor&>(&m_Matrix.matrix());
                Eigen::SimplicialLDLT<HSparseMatrixColMajor> solver;
                solver.compute(matrix);

                if (solver.info() != Eigen::Success)
                {
#ifdef _DEBUG
                    std::cerr << "LinearSolver LDLT Decomposition failed" << std::endl;
#endif
                    return -1;
                }
                x = solver.solve(b);
                if (!solver.info() != Eigen::Success)
                {
#ifdef _DEBUG
                    std::cerr << "LinearSolver LDLT Solving failed" << std::endl;
#endif
                    return -1;
                }
#ifdef _DEBUG
                std::cout << "LinearSolver Solving succeeded" << std::endl;
#endif
                return 0;
            }

        private:
            HMatrixX &m_Matrix;
        };

        class SimpleConjugateGradientSolver : public LinearSolverBase
        {
        public:
            SimpleConjugateGradientSolver(HMatrixX& matrix)
                : m_Matrix(matrix)
            {
            }

            int Solve(HVectorX& x, const HVectorX& b) override
            {
                const HSparseMatrixRowMajor& matrix = smart_cast<HSparseMatrixRowMajor&>(&m_Matrix.matrix());
                Eigen::ConjugateGradient<HSparseMatrixRowMajor, Eigen::Upper> solver;
                solver.setTolerance(1e-7);
                solver.setMaxIterations(10000);
                solver.compute(matrix);

                if (solver.info() != Eigen::Success)
                {
#ifdef _DEBUG
                    std::cerr << "ConjugateGradientSolver Decomposition failed" << std::endl;
#endif
                    return -1;
                }
                x = solver.solve(b);
                if (!solver.info() != Eigen::Success)
                {
#ifdef _DEBUG
                    std::cerr << "ConjugateGradientSolver Solving failed" << std::endl;
#endif
                    return -1;
                }
#ifdef _DEBUG
                std::cout << "ConjugateGradientSolver Solving succeeded" << std::endl;
#endif
                return 0;
            }
        private:
            HMatrixX& m_Matrix;
        };
    } // namespace SolverTool
} // namespace MathLib
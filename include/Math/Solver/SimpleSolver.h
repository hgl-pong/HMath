#pragma once
#include <Math/Math.h>
#include <Math/Solver/SolverCommon.h>

namespace MathLib
{
    namespace SolverTool
    {
        class SimpleLinearSolver : virtual public LinearSolverBase
        {
        public:
            LinearSolver(HMatrixX &matrix)
                : m_Matrix(matrix)
            {
            }

            int Solve(HVectorX &x, const HVectorX &b) override
            {
                const HSparseMatrix &matrix = smart_cast<HSparseMatrix &>(m_Matrix).getEigenMatrix();
                Eigen::SimplicialLDLT<HSparseMatrix> solver;
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

        class SimpleConjugateGradientSolver : virtual public LinearSolverBase
        {
        public:
            SimpleConjugateGradientSolver(HMatrixX &matrix)
                : m_Matrix(matrix)
            {
            }

            int Solve(HVectorX &x, const HVectorX &b) override
            {
                const HSparseMatrixRowMajor &matrix = smart_cast<HSparseMatrixRowMajor&>(m_Matrix).getEigenMatrix();
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
            };
        } // namespace SolverTool
    } // namespace MathLib
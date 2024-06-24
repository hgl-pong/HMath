#pragma once
#include <Math/Math.h>
#ifdef USE_OPENMP
#include <omp.h>
#include <mutex>
#define HFOR_EACH(loop) \
    _Pragma("omp parallel for") for (loop)
#else
#define HFOR_EACH(loop) \
    for (loop)
#endif

namespace MathLib
{
    namespace Parallel
    {
        template <typename IntType, typename = std::enable_if_t<std::is_integral<IntType>::value>>
        using ParallelFunction = std::function<void(IntType)>;
        template <typename IntType, typename = std::enable_if_t<std::is_integral<IntType>::value>>
        using ParallelFunction2 = std::function<void(IntType, IntType)>;
        template <typename IntType, typename = std::enable_if_t<std::is_integral<IntType>::value>>
        using ParallelFunction3 = std::function<void(IntType, IntType, IntType)>;

        template <typename IntType, typename = std::enable_if_t<std::is_integral<IntType>::value>>
        void ParallelFor(IntType start, IntType end, const ParallelFunction<IntType> &func)
        {
            HFOR_EACH(IntType i = start; i < end; i++)
            {
                func(i);
            }
        }

        template <typename IntType, typename = std::enable_if_t<std::is_integral<IntType>::value>>
        void SafeParallelFor(IntType start, IntType end, const ParallelFunction<IntType>&func)
        {
#ifdef USE_OPENMP
            std::mutex mutex;

            HFOR_EACH(IntType i = start; i < end; i++)
            {
                mutex.lock();
                func(i);
                mutex.unlock();
            }
#else
            ParallelFor(start, end, func);
#endif
        }

        template <typename IntType, typename = std::enable_if_t<std::is_integral<IntType>::value>>
        void ParallelFor(IntType count, const ParallelFunction<IntType>&func)
        {
            ParallelFor<IntType>(0, count, func);
        }

        template <typename IntType, typename = std::enable_if_t<std::is_integral<IntType>::value>>
        void SafeParallelFor(IntType count, const ParallelFunction<IntType>&func)
        {
            SafeParallelFor<IntType>(0, count, func);
        }

        template <typename IntType, typename = std::enable_if_t<std::is_integral<IntType>::value>>
        void ParallelFor(const std::vector<IntType> &indices, const ParallelFunction<IntType>&func)
        {
            ParallelFor<IntType>(0, indices.size(), [&](IntType i)
                                 { func(indices[i]); });
        }

        template <typename IntType, typename = std::enable_if_t<std::is_integral<IntType>::value>>
        void SafeParallelFor(const std::vector<IntType> &indices, const ParallelFunction<IntType>&func)
        {
            SafeParallelFor<IntType>(0, indices.size(), [&](IntType i)
                                     { func(indices[i]); });
        }

        template <typename IntType, typename = std::enable_if_t<std::is_integral<IntType>::value>>
        void ParallelFor(IntType begin0, IntType end0, IntType begin1, IntType end1, const ParallelFunction2<IntType>&func)
        {
            for (IntType i = begin0; i < end0; i++)
            {
                ParallelFor<IntType>(begin1, end1, [&](IntType j)
                                     { func(i, j); });
            }
        }

        template <typename IntType, typename = std::enable_if_t<std::is_integral<IntType>::value>>
        void SafeParallelFor(IntType begin0, IntType end0, IntType begin1, IntType end1, const ParallelFunction2<IntType>&func)
        {
#ifdef USE_OPENMP
            std::mutex mutex;

            for (IntType i = begin0; i < end0; i++)
            {
                for (IntType j = begin1; j < end1; j++)
                {
                    mutex.lock();
                    func(i, j);
                    mutex.unlock();
                }
            }
#else
            ParallelFor(begin0, end0, begin1, end1, func);
#endif
        }

        template <typename IntType, typename = std::enable_if_t<std::is_integral<IntType>::value>>
        void ParallelFor(IntType begin0, IntType end0, IntType begin1, IntType end1, IntType begin2, IntType end2, const ParallelFunction3<IntType>&func)
        {
            for (IntType i = begin0; i < end0; i++)
            {
                ParallelFor<IntType>(begin1, end1, [&](IntType j)
                                     { ParallelFor<IntType>(begin2, end2, [&](IntType k)
                                                            { func(i, j, k); }); });
            }
        }

        template <typename IntType, typename = std::enable_if_t<std::is_integral<IntType>::value>>
        void SafeParallelFor(IntType begin0, IntType end0, IntType begin1, IntType end1, IntType begin2, IntType end2, const ParallelFunction3<IntType>&func)
        {
#ifdef USE_OPENMP
            std::mutex mutex;

            for (IntType i = begin0; i < end0; i++)
            {
                for (IntType j = begin1; j < end1; j++)
                {
                    for (IntType k = begin2; k < end2; k++)
                    {
                        mutex.lock();
                        func(i, j, k);
                        mutex.unlock();
                    }
                }
            }
#else
            ParallelFor(begin0, end0, begin1, end1, begin2, end2, func);
#endif
        }

    } // namespace Parallel

} // namespace MathLib

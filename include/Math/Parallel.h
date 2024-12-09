#pragma once
#include <Math/Math.h>
#ifdef USE_TBB
#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>
#include <tbb/blocked_range2d.h>
#include <tbb/blocked_range3d.h>
#include <tbb/parallel_for.h>
#define DEFAULT_GRAIN_SIZE 1000
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
#ifdef USE_TBB
            tbb::parallel_for(tbb::blocked_range<IntType>(start, end),
                [&](const tbb::blocked_range<IntType>& range) {
                    for (IntType i = range.begin(); i != range.end(); ++i) {
                        func(i);
                    }
                });
#else
            for (IntType i = start; i < end; i++) {
                func(i);
            }
#endif

        }

        template <typename IntType, typename = std::enable_if_t<std::is_integral<IntType>::value>>
        void SafeParallelFor(IntType start, IntType end, const ParallelFunction<IntType>&func)
        {
#ifdef USE_TBB
            tbb::parallel_for(
                tbb::blocked_range<IntType>(start, end, DEFAULT_GRAIN_SIZE),
                [&](const tbb::blocked_range<IntType>& range) {
                    for (IntType i = range.begin(); i != range.end(); ++i) {
                        tbb::mutex mutex;
                        tbb::mutex::scoped_lock lock(mutex);
                        func(i);
                    }
                }
            );
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
#ifdef USE_TBB
            tbb::parallel_for(
                tbb::blocked_range<size_t>(0, indices.size(), DEFAULT_GRAIN_SIZE),
                [&](const tbb::blocked_range<size_t>& range) {
                    for (size_t i = range.begin(); i != range.end(); ++i) {
                        func(indices[i]);
                    }
                }
            );
#else
            ParallelFor<IntType>(0, static_cast<IntType>(indices.size()),
                [&](IntType i) { func(indices[i]); });
#endif
        }

        template <typename IntType, typename = std::enable_if_t<std::is_integral<IntType>::value>>
        void SafeParallelFor(const std::vector<IntType> &indices, const ParallelFunction<IntType>&func)
        {
#ifdef USE_TBB
            tbb::parallel_for(
                tbb::blocked_range<size_t>(0, indices.size(), DEFAULT_GRAIN_SIZE),
                [&](const tbb::blocked_range<size_t>& range) {
                    for (size_t i = range.begin(); i != range.end(); ++i) {
                        tbb::mutex mutex;
                        tbb::mutex::scoped_lock lock(mutex);
                        func(indices[i]);
                    }
                }
            );
#else
            SafeParallelFor<IntType>(0, static_cast<IntType>(indices.size()),
                [&](IntType i) { func(indices[i]); });
#endif
        }

        template <typename IntType, typename = std::enable_if_t<std::is_integral<IntType>::value>>
        void ParallelFor(IntType begin0, IntType end0, IntType begin1, IntType end1, const ParallelFunction2<IntType>&func)
        {
#ifdef USE_TBB
            tbb::parallel_for(
                tbb::blocked_range2d<IntType>(
                    begin1, end1, DEFAULT_GRAIN_SIZE,
                    begin0, end0, DEFAULT_GRAIN_SIZE
                ),
                [&](const tbb::blocked_range2d<IntType>& range) {
                    for (IntType i = range.rows().begin(); i != range.rows().end(); ++i) {
                        for (IntType j = range.cols().begin(); j != range.cols().end(); ++j) {
                            func(i, j);
                        }
                    }
                }
            );
#else
            for (IntType i = begin1; i < end1; i++) {
                for (IntType j = begin0; j < end0; j++) {
                    func(i, j);
            }
        }
#endif
        }

        template <typename IntType, typename = std::enable_if_t<std::is_integral<IntType>::value>>
        void SafeParallelFor(IntType begin0, IntType end0, IntType begin1, IntType end1, const ParallelFunction2<IntType>&func)
        {
#ifdef USE_TBB
            tbb::parallel_for(
                tbb::blocked_range2d<IntType>(
                    begin1, end1, DEFAULT_GRAIN_SIZE,
                    begin0, end0, DEFAULT_GRAIN_SIZE
                ),
                [&](const tbb::blocked_range2d<IntType>& range) {
                    for (IntType i = range.rows().begin(); i != range.rows().end(); ++i) {
                        for (IntType j = range.cols().begin(); j != range.cols().end(); ++j) {
                            tbb::mutex mutex;
                            tbb::mutex::scoped_lock lock(mutex);
                            func(i, j);
                        }
                    }
                        }
            );
#else
            ParallelFor(begin0, end0, begin1, end1, func);
#endif
        }

        template <typename IntType, typename = std::enable_if_t<std::is_integral<IntType>::value>>
        void ParallelFor(IntType begin0, IntType end0, IntType begin1, IntType end1, IntType begin2, IntType end2, const ParallelFunction3<IntType>&func)
        {
#ifdef USE_TBB
            tbb::parallel_for(
                tbb::blocked_range3d<IntType>(
                    begin2, end2, DEFAULT_GRAIN_SIZE,
                    begin1, end1, DEFAULT_GRAIN_SIZE,
                    begin0, end0, DEFAULT_GRAIN_SIZE
                ),
                [&](const tbb::blocked_range3d<IntType>& range) {
                    for (IntType i = range.pages().begin(); i != range.pages().end(); ++i) {
                        for (IntType j = range.rows().begin(); j != range.rows().end(); ++j) {
                            for (IntType k = range.cols().begin(); k != range.cols().end(); ++k) {
                                func(i, j, k);
                            }
                        }
                    }
                }
            );
#else
            for (IntType i = begin2; i < end2; i++) {
                for (IntType j = begin1; j < end1; j++) {
                    for (IntType k = begin0; k < end0; k++) {
                        func(i, j, k);
                    }
                }
            }
#endif
        }

        template <typename IntType, typename = std::enable_if_t<std::is_integral<IntType>::value>>
        void SafeParallelFor(IntType begin0, IntType end0, IntType begin1, IntType end1, IntType begin2, IntType end2, const ParallelFunction3<IntType>&func)
        {
#ifdef USE_TBB
            tbb::parallel_for(
                tbb::blocked_range3d<IntType>(
                    begin2, end2, DEFAULT_GRAIN_SIZE,
                    begin1, end1, DEFAULT_GRAIN_SIZE,
                    begin0, end0, DEFAULT_GRAIN_SIZE
                ),
                [&](const tbb::blocked_range3d<IntType>& range) {
                    for (IntType i = range.pages().begin(); i != range.pages().end(); ++i) {
                        for (IntType j = range.rows().begin(); j != range.rows().end(); ++j) {
                            for (IntType k = range.cols().begin(); k != range.cols().end(); ++k) {
                                tbb::mutex mutex;
                                tbb::mutex::scoped_lock lock(mutex);
                                func(i, j, k);
                            }
                        }
                    }
                        }
            );
#else
            ParallelFor(begin0, end0, begin1, end1, begin2, end2, func);
#endif
        }

    } // namespace Parallel

} // namespace MathLib

#pragma once
#include <Math/Math.h>
namespace MathLib
{
    template <typename T, typename Alloc = std::allocator<T>>
    class ObjectPool
    {
    public:
        ObjectPool() {}
        ObjectPool(std::size_t blockSize_)
        {
            reset(blockSize_);
        }
        ~ObjectPool()
        {
            clear();
        }
        template <typename... Args>
        T *construct(Args &&...args)
        {
            if (currentIndex >= blockSize)
            {
                currentBlock = alloc_traits::allocate(alloc, blockSize);
                allocations.emplace_back(currentBlock);
                currentIndex = 0;
            }
            T *object = &currentBlock[currentIndex++];
            alloc_traits::construct(alloc, object, std::forward<Args>(args)...);
            return object;
        }
        void reset(std::size_t newBlockSize)
        {
            for (auto allocation : allocations)
            {
                alloc_traits::deallocate(alloc, allocation, blockSize);
            }
            allocations.clear();
            blockSize = std::max<std::size_t>(1, newBlockSize);
            currentBlock = nullptr;
            currentIndex = blockSize;
        }
        void clear() { reset(blockSize); }

    private:
        T *currentBlock = nullptr;
        std::size_t currentIndex = 1;
        std::size_t blockSize = 1;
        std::vector<T *> allocations;
        Alloc alloc;
        typedef typename std::allocator_traits<Alloc> alloc_traits;
    };
}
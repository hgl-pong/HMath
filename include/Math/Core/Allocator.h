#ifndef HMATH_ALLOCATOR_H
#define HMATH_ALLOCATOR_H

#include <memory>
namespace Core
{
    inline const size_t CalculatePadding(const size_t baseAddress, const size_t alignment)
    {
        const size_t multiplier = (baseAddress / alignment) + 1;
        const size_t alignedAddress = multiplier * alignment;
        const size_t padding = alignedAddress - baseAddress;
        return padding;
    }

    inline size_t CalculatePaddingWithHeader(const size_t baseAddress, const size_t alignment, const size_t headerSize)
    {
        size_t padding = CalculatePadding(baseAddress, alignment);
        size_t space = headerSize;
        if (padding < space)
        {
            space -= padding;
            padding += alignment * (space % alignment > 0 ? (space / alignment + 1) : (space / alignment));
        }
        return padding;
    }

    class Allocator
    {
    public:
        Allocator(const size_t totalSize) : m_TotalSize{totalSize}, m_Used{0}, m_Peak{0} {}
        virtual ~Allocator() { m_TotalSize = 0; }
        virtual void *Allocate(const size_t size, const size_t alignment = 0) = 0;
        virtual void DeAllocate(void *ptr) = 0;

    protected:
        size_t m_TotalSize;
        size_t m_Used;
        size_t m_Peak;
    };

    class StackAllocator : public Allocator
    {
    public:
        StackAllocator(const size_t totalSize = 1024 * 1024) : Allocator{totalSize}
        {
            if (m_Start != nullptr)
            {
                free(m_Start);
                m_Start = nullptr;
            }
            m_Start = malloc(m_TotalSize);
            m_Offset = 0;
        }
        virtual ~StackAllocator()
        {
            free(m_Start);
            m_Start = nullptr;
        }

        void *Allocate(const size_t size, const size_t alignment = 0) override
        {
            const curAddress = static_cast<size_t>(m_Start) + m_Offset;
            const padding = CalculatePaddingWithHeader(curAddress, alignment, sizeof(AllocationHeader));
            if (m_Offset + padding + size > m_TotalSize)
            {
                return nullptr;
            }
            m_Offset + = padding;
            const size_t nextAddress = curAddress + padding;
            const size_t headerAddress = nextAddress - sizeof(AllocationHeader);
            AllocationHeader allocationHeader{padding};
            AllocationHeader *header = reinterpret_cast<AllocationHeader *>(headerAddress);
            header = &allocationHeader;
#ifdef _DEBUG
            printf("Allocated \t CurAddress: %p\t NextAddress: %p\t Offset: %zu\t Padding: %zu\n", (void *)currentAddress, (void *)nextAddress, m_offset, padding);
#endif
            m_Offset += size;
            m_Used = m_Offset;
            m_Peak = std::max(m_Peak, m_Used);
            return reinterpret_cast<void *>(nextAddress);
        }

        void DeAllocate(void *ptr) override
        {
            const size_t currAddress = static_cast<size_t>(ptr);
            const size_t headerAddress = currAddress - sizeof(AllocationHeader);
            AllocationHeader *header = reinterpret_cast<AllocationHeader *>(headerAddress);
            m_Offset = currAddress - header->mPadding + sizeof(AllocationHeader);
            m_Used = m_Offset;
#ifdef _DEBUG
            printf("DeAllocated \t CurAddress: %p\t NextAddress: %p\t Offset: %zu\t Padding: %zu\n", (void *)currAddress, (void *)nextAddress, m_offset, padding);
#endif
        }

        virtual void Reset()
        {
            m_Offset = 0;
            m_Used = 0;
            m_Peak = 0;
        }

    private:
        struct AllocationHeader
        {
            char mPadding;
        };

    protected:
        void *m_Start = nullptr;
        size_t m_Offset;
    };

    class LinearAllocator : public Allocator
    {
    public:
        LinearAllocator(const size_t totalSize = 1024 * 1024)
            : Allocator{totalSize}
        {
            if (m_Start != nullptr)
            {
                free(m_Start);
                m_Start = nullptr;
            }
            m_Start = malloc(m_TotalSize);
            m_Offset = 0;
        }
        virtual ~LinearAllocator()
        {
            free(m_Start);
            m_Start = nullptr;
        }
        virtual void *Allocate(const size_t size, const size_t alignment = 0) override
        {
            size_t padding = 0;
            size_t paddedAddress = 0;
            const size_t currentAddress = static_cast<size_t>(m_Start) + m_Offset;

            if (alignment != 0 && m_Offset % alignment != 0)
            {
                padding = CalculatePadding(currentAddress, alignment);
            }
            if (m_Offset + padding + size > m_TotalSize)
            {
                return nullptr;
            }
            m_Offset += padding;
            const size_t nextAddress = currentAddress + padding;
            m_Offset += size;
#ifdef _DEBUG
            printf("Allocated \t CurAddress: %p\t NextAddress: %p\t Offset: %zu\t Padding: %zu\n", (void *)currentAddress, (void *)nextAddress, m_offset, padding);
#endif
            m_Used = m_Offset;
            m_Peak = std::max(m_Peak, m_Used);
            return reinterpret_cast<void *>(nextAddress);
        }
        virtual void DeAllocate(void *ptr) override
        {
            const size_t currAddress = static_cast<size_t>(ptr);
            m_Offset = currAddress - sizeof(AllocationHeader);
            m_Used = m_Offset;
#ifdef _DEBUG
            printf("DeAllocated \t CurAddress: %p\t NextAddress: %p\t Offset: %zu\t Padding: %zu\n", (void *)currAddress, (void *)nextAddress, m_offset, padding);
#endif
        }
        virtual void Reset()
        {
            m_Offset = 0;
            m_Used = 0;
            m_Peak = 0;
        }

    private:
        void *m_Start = nullptr;
        size_t m_Offset = 0;
    };
}

static Core::StackAllocator gDefaultAllocator;

#ifndef HNEW
#define HNEW()
#endif
#ifndef HDELETE
#define HDELETE()
#endif

#endif // HMATH_ALLOCATOR_H

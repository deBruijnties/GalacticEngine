#pragma once
#include <vector>
#include <cstddef>
#include <cstring>
#include <type_traits>
#include <iostream>

class StructuredBuffer
{
public:
    StructuredBuffer(unsigned int bindingPoint, size_t strideBytes);
    ~StructuredBuffer();

    void SetActiveCount(size_t count)
    {
        activeCount = std::min(count, capacity);
    }

    size_t GetActiveCount() const { return activeCount; }

    void Allocate(size_t instanceCount, unsigned int usage = 0x88E8); // 0x88E8 = GL_DYNAMIC_DRAW
    void Upload();

    template<typename T>
    void Set(size_t index, const T& value)
    {
        static_assert(std::is_trivially_copyable_v<T>);

        if (index >= capacity)
        {
            std::cout << "StructuredBuffer OOB write: index=" << index
                << " capacity=" << capacity << "\n";
            return;
        }

        std::memcpy(cpuData.data() + index * stride, &value, sizeof(T));
    }

    void* GetRawPtr(size_t index = 0);
    void Bind() const;


    size_t GetCapacity() const { return capacity; }
    size_t GetStride() const;

private:

    size_t capacity = 0;
    size_t activeCount = 0;

    unsigned int id = 0;
    unsigned int binding;
    size_t stride;

    std::vector<std::byte> cpuData;
};

#pragma once
#include <vector>
#include <cstddef>
#include <cstring>
#include <type_traits>

class StructuredBuffer
{
public:
    StructuredBuffer(unsigned int bindingPoint, size_t strideBytes);
    ~StructuredBuffer();

    void Allocate(size_t instanceCount, unsigned int usage = 0x88E8); // 0x88E8 = GL_DYNAMIC_DRAW
    void Upload();

    template<typename T>
    void Set(size_t index, const T& value)
    {
        static_assert(std::is_trivially_copyable_v<T>);
        std::memcpy(
            cpuData.data() + index * stride,
            &value,
            sizeof(T)
        );
    }

    void* GetRawPtr(size_t index = 0);
    void Bind() const;

    size_t GetInstanceCount() const;
    size_t GetStride() const;

private:
    unsigned int id = 0;
    unsigned int binding;
    size_t stride;
    size_t count = 0;

    std::vector<std::byte> cpuData;
};

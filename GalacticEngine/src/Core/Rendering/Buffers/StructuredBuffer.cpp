#include "pch.h"
#include <GLAD/glad.h>
#include "StructuredBuffer.h"

StructuredBuffer::StructuredBuffer(unsigned int bindingPoint, size_t strideBytes)
    : binding(bindingPoint), stride(strideBytes)
{
    glGenBuffers(1, &id);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, id);
}

StructuredBuffer::~StructuredBuffer()
{
    if (id != 0)
        glDeleteBuffers(1, &id);
}

void StructuredBuffer::Allocate(size_t instanceCount, unsigned int usage)
{
    count = instanceCount;
    cpuData.resize(count * stride);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, id);
    glBufferData(
        GL_SHADER_STORAGE_BUFFER,
        cpuData.size(),
        nullptr,
        usage
    );
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void StructuredBuffer::Upload()
{
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, id);
    glBufferSubData(
        GL_SHADER_STORAGE_BUFFER,
        0,
        cpuData.size(),
        cpuData.data()
    );
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void* StructuredBuffer::GetRawPtr(size_t index)
{
    return cpuData.data() + index * stride;
}

void StructuredBuffer::Bind() const
{
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, id);
}

size_t StructuredBuffer::GetInstanceCount() const
{
    return count;
}

size_t StructuredBuffer::GetStride() const
{
    return stride;
}

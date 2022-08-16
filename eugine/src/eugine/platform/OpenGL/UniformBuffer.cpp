//
// Created by aiden on 7/25/22.
//

#include "UniformBuffer.h"

#include "glad/glad.h"
#include "error.h"

namespace eg::rendering::GLWrapper {
    UniformBuffer::UniformBuffer(u32 size, VertexBuffer::UsageHints usageHints) {
        GLCall(glGenBuffers(1, &m_ID));
        GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_ID));
        GLCall(glBufferData(GL_UNIFORM_BUFFER, size, NULL,
                            usageHints == VertexBuffer::VB_USAGE_HINT_DYNAMIC ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));
    }

    UniformBuffer::UniformBuffer(void* data, u32 size, VertexBuffer::UsageHints usageHints) {
        GLCall(glGenBuffers(1, &m_ID));
        GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_ID));
        GLCall(glBufferData(GL_UNIFORM_BUFFER, size, data,
                            usageHints == VertexBuffer::VB_USAGE_HINT_DYNAMIC ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));
    }

    void UniformBuffer::bind() {
        GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_ID));
    }

    void UniformBuffer::unBind() {
        GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
    }

    UniformBuffer::~UniformBuffer() {
        GLCall(glDeleteBuffers(1, &m_ID));
    }

    void UniformBuffer::setData(void* data, u32 size) {
        bind();
        GLCall(glBufferSubData(GL_UNIFORM_BUFFER, 0, size, data))
    }
}
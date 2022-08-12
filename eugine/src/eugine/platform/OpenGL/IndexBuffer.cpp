//
// Created by aiden on 4/24/22.
//

#include "IndexBuffer.h"
#include "error.h"

#include "glad/glad.h"

namespace eg::rendering::GLWrapper {
    IndexBuffer::IndexBuffer(u16* data, u32 count, rendering::VertexBuffer::UsageHints usageHints) : m_maxCount(count) {
        GLCall(glGenBuffers(1, &m_ID));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID));
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(u16), data,
                            usageHints == VertexBuffer::VB_USAGE_HINT_DYNAMIC ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));

        m_elementCount = count;
    }

    void IndexBuffer::bind() {
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID));
    }

    void IndexBuffer::unBind() {
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    }

    IndexBuffer::~IndexBuffer() {
        GLCall(glDeleteBuffers(1, &m_ID));
    }

    void IndexBuffer::setData(const u16* data, u32 count) {
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID));
        GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, count * sizeof(u16), data));
    }
}
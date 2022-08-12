#include "VertexBuffer.h"
#include "error.h"
#include "glad/glad.h"

#include <utility>

namespace eg::rendering::GLWrapper {

    VertexBuffer::VertexBuffer(const void* data, u32 size, rendering::VertexBuffer::UsageHints usageHints, rendering::VertexBufferLayout layout)
            : m_layout(layout) {
        GLCall(glGenBuffers(1, &m_ID));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_ID));
        GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, usageHints == VB_USAGE_HINT_DYNAMIC ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW));

        m_vertexArray.setBuffer(*this);
    }

    void VertexBuffer::setData(void* data, u32 size) {
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_ID));
        GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
    }

    VertexBuffer::~VertexBuffer() { GLCall(glDeleteBuffers(1, &m_ID)); }

    void VertexBuffer::bind() { m_vertexArray.bind(); }

    void VertexBuffer::unBind() { m_vertexArray.bind(); }

} // namespace eg::rendering::GLWrapper

#include "VertexBuffer.h"
#include "eugine/platform/OpenGL/wrapper/error.h"
#include <glad/glad.h>

#include <utility>

namespace eg {
    namespace GLWrapper {

        VertexBuffer::VertexBuffer(const void *data, u32 size, VertexBufferLayout layout)
            : m_layout(std::move(layout)) {
            GLCall(glGenBuffers(1, &m_ID));
            GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_ID));
            GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));
        }

        void VertexBuffer::setData(void *data, u32 size) {
            GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_ID));
            GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));
        }

        VertexBuffer::~VertexBuffer() { GLCall(glDeleteBuffers(1, &m_ID)); }

        void VertexBuffer::bind() const { GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_ID)); }

        void VertexBuffer::unBind() const { GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0);); }

    } // namespace glwrapper
} // namespace eg

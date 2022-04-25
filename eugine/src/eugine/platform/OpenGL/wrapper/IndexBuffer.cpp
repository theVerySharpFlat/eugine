//
// Created by aiden on 4/24/22.
//

#include "IndexBuffer.h"
#include "error.h"

#include <glad/glad.h>

namespace eg::GLWrapper {
    IndexBuffer::IndexBuffer(u32 *data, u32 size) {
        GLCall(glGenBuffers(1, &m_ID));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID));
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));

        m_elementCount = size / sizeof(GLuint);
    }

    void IndexBuffer::bind() const {
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID));
    }

    void IndexBuffer::unBind() const {
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    }

    IndexBuffer::~IndexBuffer() {
        GLCall(glDeleteBuffers(1, &m_ID));
    }
}
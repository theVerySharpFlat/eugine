//
// Created by aiden on 4/23/22.
//

#include "VertexArray.h"
#include "error.h"

#include <glad/glad.h>

namespace eg {
    namespace GLWrapper {
        VertexArray::VertexArray() {
            GLCall(glCreateVertexArrays(1, &m_ID));
        }

        VertexArray::~VertexArray() {
            GLCall(glDeleteVertexArrays(1, &m_ID));
        }

        void VertexArray::setBuffer(const VertexBuffer &vertexBuffer) {
            bind();
            vertexBuffer.bind();

            u64 offset = 0;
            const auto &attributes = vertexBufferLayout.getAttributes();
            for (int i = 0; i < attributes.size(); i++) {
                GLCall(glVertexAttribPointer(i, attributes[i].count, attributes[i].type, GL_FALSE,
                                             vertexBufferLayout.getStride(),
                                             (void*) offset));
                GLCall(glEnableVertexAttribArray(i));

                offset += attributes[i].count * getSizeOfType(attributes[i].type);
            }
        }

        void VertexArray::bind() const {
            GLCall(glBindVertexArray(m_ID));
        }

        void VertexArray::unBind() const {
            GLCall(glBindVertexArray(0));
        }
    }
}
//
// Created by aiden on 4/23/22.
//

#include "VertexBufferLayout.h"
#include "glad/glad.h"

namespace eg {
    namespace GLWrapper {

        u32 getSizeOfType(u32 type) {
            switch (type) {
                case GL_FLOAT: return sizeof(GLfloat);
                case GL_UNSIGNED_INT: return sizeof(GLuint);
                case GL_INT: return sizeof(GLint);
                case GL_UNSIGNED_SHORT: return sizeof(GLushort);
                case GL_SHORT: return sizeof(GLshort);
                case GL_UNSIGNED_BYTE: return sizeof(GLubyte);
                case GL_BYTE: return sizeof(GLbyte);
                default:
                    EG_ASSERT(false, "Unknown type \"{0:x}\"", type);
                    return 0;
            }
        }

        VertexBufferLayout::VertexBufferLayout(u32 num)
                : m_numAttributes(num), m_attributes(num) {}

        void VertexBufferLayout::setAttribute(u32 index, VertexAttrib attrib) {
            EG_ASSERT(index >= 0 && index < m_numAttributes, "VertexAttrib out of index!");
            EG_ASSERT(getSizeOfType(attrib.type) != 0, "Bad type");
            m_attributes[index] = attrib;
        }

        u32 VertexBufferLayout::getStride() const {
            u32 stride = 0;
            for (auto& attr : m_attributes) {
                EG_ASSERT(attr.count != 0, "Uninitialized VertexAttribute!");
                stride += getSizeOfType(attr.type) * attr.count;
            }
            return stride;
        }
    }
}
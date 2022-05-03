#include "VertexBufferLayout.h"

namespace eg::rendering {
    VertexBufferLayout::VertexBufferLayout(u32 num)
            : m_numAttributes(num), m_attributes(num) {}

    void VertexBufferLayout::setAttribute(u32 index, VertexAttrib attrib) {
        EG_ASSERT(index >= 0 && index < m_numAttributes, "VertexAttrib out of index!");
        EG_ASSERT(getSizeOfType(attrib.type) != 0, "Bad type");
        trace("size of type: {}", getSizeOfType(attrib.type));
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
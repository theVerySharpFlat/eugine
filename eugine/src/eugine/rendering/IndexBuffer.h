#ifndef EG_RENDERING_INDEX_BUFFER_H
#define EG_RENDERING_INDEX_BUFFER_H

#include "VertexBuffer.h"

namespace eg::rendering {
    class IndexBuffer {
    public:
        static Ref<IndexBuffer> create(u16* data, u32 count, VertexBuffer::UsageHints usageHints);

        virtual void setData(const u16* data, u32 count) = 0;

        virtual u32 getElementCount() const = 0;
        virtual u32 getMaxElementCount() const = 0;
    };
}

#endif
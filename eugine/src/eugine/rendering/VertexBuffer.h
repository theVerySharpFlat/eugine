//
// Created by aiden on 5/1/22.
//

#ifndef EUGINE_VERTEXBUFFER_H
#define EUGINE_VERTEXBUFFER_H

#include <eugine/rendering/VertexBufferLayout.h>

namespace eg::rendering {
    class VertexBuffer {
    public:
        enum UsageHints {
            VB_USAGE_HINT_STATIC = 0,
            VB_USAGE_HINT_DYNAMIC
        };

        static Ref<VertexBuffer> create(void* data, u32 size, const VertexBufferLayout& layout, VertexBuffer::UsageHints usageHints);

        virtual void setLayout(const VertexBufferLayout& layout) = 0;
        virtual const VertexBufferLayout& getLayout() const = 0;

        virtual void setData(void* data, u32 size) = 0;
    };
}

#endif //EUGINE_VERTEXBUFFER_H

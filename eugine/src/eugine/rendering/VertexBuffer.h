//
// Created by aiden on 5/1/22.
//

#ifndef EUGINE_VERTEXBUFFER_H
#define EUGINE_VERTEXBUFFER_H

#include <eugine/rendering/VertexBufferLayout.h>

namespace eg::rendering {
    class VertexBuffer {
    public:
        static Ref<VertexBuffer> create(void* data, u32 size, const VertexBufferLayout& layout);
        virtual void bind() const = 0;
        virtual void unBind() const = 0;
        virtual void setData(void* data, u32 size) = 0;
    };
}

#endif //EUGINE_VERTEXBUFFER_H

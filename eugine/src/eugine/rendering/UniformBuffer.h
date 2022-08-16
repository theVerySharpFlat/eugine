//
// Created by aiden on 7/25/22.
//

#ifndef EUGINE_UNIFORMBUFFER_H
#define EUGINE_UNIFORMBUFFER_H

#include "VertexBuffer.h"

namespace eg::rendering {
    class UniformBuffer {
    public:
        static Ref<UniformBuffer> create(void* data, u32 size, VertexBuffer::UsageHints usageHints);
        virtual void setData(void* data, u32 size) = 0;
        virtual ~UniformBuffer() {};
    };
}


#endif //EUGINE_UNIFORMBUFFER_H

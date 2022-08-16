//
// Created by aiden on 7/25/22.
//

#ifndef EUGINE_GLWRAPPER_UNIFORMBUFFER_H
#define EUGINE_GLWRAPPER_UNIFORMBUFFER_H

#include "eugine/rendering/UniformBuffer.h"

namespace eg::rendering::GLWrapper {
    class UniformBuffer : public ::eg::rendering::UniformBuffer {
    public:
        UniformBuffer(u32 size, VertexBuffer::UsageHints usageHints);
        UniformBuffer(void* data, u32 size, VertexBuffer::UsageHints usageHints);

        void bind();
        void unBind();

        ~UniformBuffer();

        void setData(void* data, u32 size) override;

    private:
        friend class Shader;
        u32 m_ID;
    };
}


#endif //EUGINE_GLWRAPPER_UNIFORMBUFFER_H

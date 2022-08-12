//
// Created by aiden on 4/24/22.
//

#ifndef EUGINE_INDEXBUFFER_H
#define EUGINE_INDEXBUFFER_H

#include "eugine/rendering/IndexBuffer.h"

namespace eg::rendering::GLWrapper {
    class IndexBuffer : public ::eg::rendering::IndexBuffer{
    public:
        IndexBuffer(u16 *data, u32 size, rendering::VertexBuffer::UsageHints usageHints);
        ~IndexBuffer();

        u32 getElementCount() const override { return m_elementCount; }
        u32 getMaxElementCount() const override { return m_maxCount; }

        void bind();
        void unBind();

        void setData(const u16 *data, u32 count) override;

    private:
        u32 m_elementCount = 0;
        const u32 m_maxCount;
        u32 m_ID = 0;
    };
}

#endif //EUGINE_INDEXBUFFER_H

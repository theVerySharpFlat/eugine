//
// Created by aiden on 4/24/22.
//

#ifndef EUGINE_INDEXBUFFER_H
#define EUGINE_INDEXBUFFER_H

#include <eugine/rendering/IndexBuffer.h>

namespace eg::GLWrapper {
    class IndexBuffer : public ::eg::rendering::IndexBuffer{
    public:
        IndexBuffer(u32 *data, u32 size);
        ~IndexBuffer();

        u32 getElementCount() const override { return m_elementCount; }

        void bind() const override;

        void unBind() const override;

    private:
        u32 m_elementCount = 0;
        u32 m_ID = 0;
    };
}

#endif //EUGINE_INDEXBUFFER_H

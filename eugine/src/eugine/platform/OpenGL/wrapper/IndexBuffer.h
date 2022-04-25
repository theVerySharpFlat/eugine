//
// Created by aiden on 4/24/22.
//

#ifndef EUGINE_INDEXBUFFER_H
#define EUGINE_INDEXBUFFER_H

namespace eg::GLWrapper {
    class IndexBuffer {
    public:
        IndexBuffer(u32 *data, u32 size);

        u32 elementCount() const { return m_elementCount; }

        void bind() const;

        void unBind() const;

    private:
        u32 m_elementCount = 0;
        u32 m_ID = 0;
    };
}

#endif //EUGINE_INDEXBUFFER_H

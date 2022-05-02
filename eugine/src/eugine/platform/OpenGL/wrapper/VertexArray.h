//
// Created by aiden on 4/23/22.
//

#ifndef EUGINE_VERTEXARRAY_H
#define EUGINE_VERTEXARRAY_H

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace eg {
    namespace GLWrapper {
        class VertexArray {
        public:
            VertexArray();
            ~VertexArray();

            void setBuffer(const VertexBuffer& vertexBuffer);

            void bind() const;
            void unBind() const;

        private:
            u32 m_ID;
        };
    }
}

#endif //EUGINE_VERTEXARRAY_H

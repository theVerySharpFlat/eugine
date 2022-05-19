//
// Created by aiden on 4/23/22.
//

#ifndef EUGINE_VERTEXARRAY_H
#define EUGINE_VERTEXARRAY_H

#include <eugine/rendering/VertexArray.h>
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "eugine/rendering/VertexBuffer.h"

namespace eg {
    namespace GLWrapper {
        class VertexArray : public ::eg::rendering::VertexArray{
        public:

            VertexArray();
            VertexArray(const ::eg::rendering::VertexBuffer& vertexBuffer);
            ~VertexArray();

            void setBuffer(const ::eg::rendering::VertexBuffer& vertexBuffer) override;

            void bind() const override;
            void unBind() const override;

        private:
            u32 m_ID;

            // Non-virtual definition of VertexArray::setBuffer because this
            // will be called in the constructor
            void _setBuffer(const ::eg::rendering::VertexBuffer& vertexBuffer);
        };
    }
}

#endif //EUGINE_VERTEXARRAY_H

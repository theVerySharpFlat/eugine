//
// Created by aiden on 4/23/22.
//

#ifndef EUGINE_VERTEXARRAY_H
#define EUGINE_VERTEXARRAY_H

#include "VertexBufferLayout.h"
#include "eugine/rendering/VertexBuffer.h"

namespace eg::rendering::GLWrapper {
    class VertexBuffer;
    class VertexArray {
    public:

        VertexArray();

        VertexArray(GLWrapper::VertexBuffer& vertexBuffer);

        ~VertexArray();

        void setBuffer(GLWrapper::VertexBuffer& VertexBuffer);

        void bind();

        void unBind();

    private:
        u32 m_ID;

        // Non-virtual definition of VertexArray::setBuffer because this
        // will be called in the constructor
        void _setBuffer(GLWrapper::VertexBuffer& vertexBuffer);
    };
}

#endif //EUGINE_VERTEXARRAY_H

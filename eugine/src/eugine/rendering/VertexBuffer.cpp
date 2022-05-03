//
// Created by aiden on 5/1/22.
//

#include "VertexBuffer.h"
#include "eugine/rendering/VertexBufferLayout.h"
#include <eugine/platform/OpenGL/wrapper/VertexBuffer.h>

namespace eg::rendering {

Ref<VertexBuffer> VertexBuffer::create(void *data, u32 size, const VertexBufferLayout& layout) {
    return createRef<GLWrapper::VertexBuffer>(data, size, layout);
}

}
//
// Created by aiden on 5/1/22.
//

#include "VertexBuffer.h"
#include <eugine/platform/OpenGL/wrapper/VertexBuffer.h>

eg::Ref<eg::VertexBuffer> eg::VertexBuffer::create(void *data, u32 size) {
    return createRef<GLWrapper::VertexBuffer>(data, size);
}

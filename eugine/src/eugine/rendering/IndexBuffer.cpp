#include "IndexBuffer.h"
#include "eugine/platform/OpenGL/wrapper/IndexBuffer.h"

namespace eg::rendering {
    Ref<IndexBuffer> IndexBuffer::create(u32* data, u32 size) {
        return createRef<GLWrapper::IndexBuffer>(data, size);
    }
}
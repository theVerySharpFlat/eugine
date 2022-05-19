#include "VertexArray.h"
#include "eugine/platform/OpenGL/wrapper/VertexArray.h"

namespace eg::rendering {

Ref<VertexArray> VertexArray::create() {
    return createRef<GLWrapper::VertexArray>();
}

}
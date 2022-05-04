#include "LowLevelRenderer.h"
#include "eugine/platform/OpenGL/wrapper/Renderer.h"

namespace eg::rendering {
    Ref<LowLevelRenderer> LowLevelRenderer::create() {
        return createRef<GLWrapper::Renderer>(); 
    }
}
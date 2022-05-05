#include "eugine/platform/OpenGL/wrapper/GLAPI.h"
#include "eugine/rendering/GraphicsAPI.h"

namespace eg::rendering {
    Ref<GraphicsAPI> GraphicsAPI::create(Window& window) {
        return createRef<GLWrapper::OGLAPI>(window);
    }
}
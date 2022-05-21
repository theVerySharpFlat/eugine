#include "eugine/platform/OpenGL/wrapper/GLAPI.h"
#include "eugine/rendering/GraphicsAPI.h"

namespace eg::rendering {

    GraphicsAPIID getPreferredGraphicsAPI() {
        return EG_API_VK;
    }

    Ref<GraphicsAPI> GraphicsAPI::create(Window& window) {
        return createRef<GLWrapper::OGLAPI>(window);
    }
}
#include "eugine/platform/OpenGL/wrapper/GLAPI.h"
#include "eugine/platform/Vulkan/VKAPI.h"
#include "eugine/rendering/GraphicsAPI.h"

namespace eg::rendering {

    GraphicsAPIID getPreferredGraphicsAPI() {
        return EG_API_VK;
    }

    Ref<GraphicsAPI> GraphicsAPI::create(Window& window) {
        GraphicsAPIID api = getPreferredGraphicsAPI();
        if(api == EG_API_OGL)
            return createRef<GLWrapper::OGLAPI>(window);
        else if(api == EG_API_VK)
            return createRef<VKWrapper::VKAPI>(window);
        else
            EG_ASSERT(false, "Invalid graphics API");

        return nullptr;
    }
}
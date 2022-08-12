#include "eugine/platform/OpenGL/OpenGLAPI.h"
#include "eugine/platform/Vulkan/VKAPI.h"
#include "eugine/rendering/GraphicsAPI.h"

namespace eg::rendering {

    GraphicsAPIID getPreferredGraphicsAPI() {
        return EG_API_OGL;
    }

    GraphicsAPI* GraphicsAPI::create(Window& window) {
        GraphicsAPIID api = getPreferredGraphicsAPI();
        if(api == EG_API_OGL)
            return new GLWrapper::OpenGLAPI(window);
        else if(api == EG_API_VK)
            return new VKWrapper::VKAPI(window);
        else
            EG_ASSERT(false, "Invalid graphics API");

        return nullptr;
    }
}
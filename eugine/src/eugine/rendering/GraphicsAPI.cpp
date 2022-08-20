#include "eugine/platform/OpenGL/OpenGLAPI.h"
#include "eugine/platform/Vulkan/VKAPI.h"
#include "eugine/rendering/GraphicsAPI.h"


namespace eg::rendering {

    static GraphicsAPIID currentGraphicsAPI = EG_API_VK;

    GraphicsAPIID getPreferredGraphicsAPI() {
        return currentGraphicsAPI;
    }

    void setPreferredGraphicsAPI(GraphicsAPIID api) {
        currentGraphicsAPI = api;
    }


    GraphicsAPI* GraphicsAPI::create(Window& window) {
        GraphicsAPIID api = getPreferredGraphicsAPI();
        if(api == EG_API_OGL)
            return new GLWrapper::OpenGLAPI(window);
        else if(api == EG_API_VK)
            return new VKWrapper::VKAPI(window, true);
        else
            EG_ASSERT(false, "Invalid graphics API");

        return nullptr;
    }

    bool GraphicsAPI::vulkanInitializationSuccessful() {
        return VKWrapper::VKAPI::get()->getInitSuccess();
    }
}

#include "Texture.h"
#include "GraphicsAPI.h"
#include "eugine/platform/OpenGL/wrapper/Texture.h"
#include "eugine/platform/Vulkan/VKAPI.h"
#include "eugine/platform/Vulkan/VkTexture.h"

namespace eg::rendering {
    Ref<Texture> Texture::create(const fs::path& path) {
        if(rendering::getPreferredGraphicsAPI() == EG_API_OGL)
            return createRef<GLWrapper::Texture>(path);
        else if(rendering::getPreferredGraphicsAPI() == EG_API_VK)
            return VKWrapper::VKAPI::get()->createTexture(path.c_str());
        else
            return nullptr;
    }
}
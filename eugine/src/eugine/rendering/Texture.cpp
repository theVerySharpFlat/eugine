#include "Texture.h"
#include "GraphicsAPI.h"
#include "eugine/platform/OpenGL/Texture.h"
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

    Ref<Texture> Texture::create(const u8* data, u32 size, const char* name) {
        if(rendering::getPreferredGraphicsAPI() == EG_API_OGL)
            return createRef<GLWrapper::Texture>(data, size, name);
        else if(rendering::getPreferredGraphicsAPI() == EG_API_VK)
            return VKWrapper::VKAPI::get()->createTextureFromData(data, size, name);
        else
            return nullptr;
    }
}
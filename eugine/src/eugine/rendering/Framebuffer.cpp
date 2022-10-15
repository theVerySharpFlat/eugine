//
// Created by aiden on 10/8/22.
//

#include "Framebuffer.h"
#include "eugine/platform/Vulkan/VKAPI.h"
#include <vulkan/vulkan_core.h>

namespace eg::rendering {
    Ref<Framebuffer> Framebuffer::create(Usage usage) {
        return VKWrapper::VKAPI::get()->createFramebuffer(usage);
    }
}

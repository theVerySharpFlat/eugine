//
// Created by aiden on 7/25/22.
//

#include "UniformBuffer.h"
#include "GraphicsAPI.h"

#include "eugine/platform/Vulkan/VKAPI.h"
#include "eugine/platform/Vulkan/VkUniformBuffer.h"

namespace eg::rendering {
    Ref<UniformBuffer> UniformBuffer::create(void* data, u32 size, VertexBuffer::UsageHints usageHints) {
        if(getPreferredGraphicsAPI() == EG_API_VK)
            return rendering::VKWrapper::VKAPI::get()->createUniformBuffer(data, size, usageHints);
    }
}
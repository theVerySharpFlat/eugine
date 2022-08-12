//
// Created by aiden on 5/1/22.
//

#include "VertexBuffer.h"
#include "GraphicsAPI.h"
#include "eugine/platform/Vulkan/VKAPI.h"
#include "eugine/platform/Vulkan/VkVertexBuffer.h"
#include "eugine/rendering/VertexBufferLayout.h"
#include "eugine/platform/OpenGL/VertexBuffer.h"

namespace eg::rendering {

Ref<VertexBuffer> VertexBuffer::create(void *data, u32 size, const VertexBufferLayout& layout, VertexBuffer::UsageHints usageHints) {
    if(getPreferredGraphicsAPI() == EG_API_OGL)
        return createRef<GLWrapper::VertexBuffer>(data, size, usageHints, layout);
    else if(getPreferredGraphicsAPI() == EG_API_VK)
        return rendering::VKWrapper::VKAPI::get()->createVertexBuffer(data, size,
                                                                      const_cast<VertexBufferLayout&>(layout), usageHints);
}

}
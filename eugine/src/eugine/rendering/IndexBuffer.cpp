#include "IndexBuffer.h"
#include "eugine/platform/OpenGL/IndexBuffer.h"
#include "eugine/platform/Vulkan/VKAPI.h"
#include "eugine/platform/Vulkan/VkIndexBuffer.h"
#include "eugine/rendering/GraphicsAPI.h"

namespace eg::rendering {
    Ref<IndexBuffer> IndexBuffer::create(u16* data, u32 count, VertexBuffer::UsageHints usageHints) {
        if(getPreferredGraphicsAPI() == EG_API_OGL)
            return createRef<GLWrapper::IndexBuffer>(data, count, usageHints);
        else if(getPreferredGraphicsAPI() == EG_API_VK) {
            return rendering::VKWrapper::VKAPI::get()->createIndexBuffer(data, count, usageHints);
        } else {
            EG_ASSERT(false, "Invalid graphics API. This should never happen!");
            return nullptr;
        }
    }
}
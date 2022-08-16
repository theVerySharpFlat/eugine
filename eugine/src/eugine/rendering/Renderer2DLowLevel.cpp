//
// Created by aiden on 7/24/22.
//

#include "GraphicsAPI.h"
#include "Renderer2DLowLevel.h"
#include "eugine/platform/OpenGL/Renderer2DLowLevel.h"
#include "eugine/platform/Vulkan/VkRenderer2DLowLevel.h"

namespace eg::rendering {
    Ref<Renderer2DLowLevel> Renderer2DLowLevel::create(GraphicsAPI& graphicsAPI, Renderer2D::Settings settings, VertexBufferLayout layout) {
        if(getPreferredGraphicsAPI() == EG_API_VK)
            return eg::createRef<VKWrapper::VkRenderer2DLowLevel>((VKWrapper::VKAPI&)graphicsAPI, settings);
        else
            return eg::createRef<GLWrapper::Renderer2DLowLevel>(settings, layout);
    }
}
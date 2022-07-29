//
// Created by aiden on 7/24/22.
//

#include "Renderer2DLowLevel.h"
#include "eugine/platform/Vulkan/VkRenderer2DLowLevel.h"

namespace eg::rendering {
    Ref <Renderer2DLowLevel> Renderer2DLowLevel::create(GraphicsAPI& graphicsAPI, Renderer2D::Settings settings) {
        return eg::createRef<VKWrapper::VkRenderer2DLowLevel>((VKWrapper::VKAPI&)graphicsAPI, settings);
    }
}
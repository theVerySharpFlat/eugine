//
// Created by aiden on 7/15/22.
//

#ifndef EUGINE_VKCOMMANDBUFFERUTIL_H
#define EUGINE_VKCOMMANDBUFFERUTIL_H

#include "volk.h"

namespace eg::rendering::VKWrapper {
    VkCommandBuffer beginSingleTimeCommands(::VkDevice device, ::VkCommandPool pool);
    VkFence endSingleTimeCommands(::VkDevice device, ::VkQueue queue, VkCommandBuffer buffer);
}

#endif //EUGINE_VKCOMMANDBUFFERUTIL_H

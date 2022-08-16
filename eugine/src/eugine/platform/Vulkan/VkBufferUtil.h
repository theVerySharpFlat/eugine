//
// Created by aiden on 6/29/22.
//

#ifndef EUGINE_VKBUFFERUTIL_H
#define EUGINE_VKBUFFERUTIL_H

#include <volk.h>
#include <vk_mem_alloc.h>

namespace eg::rendering::VKWrapper {
    class VkDevice;
    namespace BufferUtil {
        enum BufferType {
            BUFFER_DYNAMIC,
            BUFFER_STATIC,
            BUFFER_STAGING
        };

        void
        createBuffer(VmaAllocator allocator, VmaMemoryUsage vmaMemoryUsage, VkBufferUsageFlags bufferUsage, BufferType bufferType, u32 size,
                     VkBuffer* buffer, VmaAllocation* allocation, VmaAllocationInfo* allocationInfo = nullptr);

        void
        copyBuffer(VkDevice& device, VkCommandPool commandPool, VkBuffer srcBuffer, VkBuffer dstBuffer,
                   VkDeviceSize size);
    }
}

#endif //EUGINE_VKBUFFERUTIL_H

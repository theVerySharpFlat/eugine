//
// Created by aiden on 6/28/22.
//

#ifndef EUGINE_VKVERTEXBUFFER_H
#define EUGINE_VKVERTEXBUFFER_H

#include <volk.h>
#include <vk_mem_alloc.h>

#include "VkDevice.h"
#include <eugine/rendering/VertexBuffer.h>

namespace eg::rendering::VKWrapper {
    class VkDevice;
    class VkVertexBuffer {
    public:
        VkVertexBuffer(VkDevice& device, VkCommandPool commandPool, VmaAllocator& allocator, void* data, u32 size, VertexBuffer::UsageHints usageHint);
        ~VkVertexBuffer();

        VkBuffer& getBuffer() { return m_buffer; }

    private:
        VmaAllocator& m_allocator;
        VkDevice& m_device;

        u32 m_maxSize;
        VertexBuffer::UsageHints m_usageHint;

        VkBuffer m_buffer = VK_NULL_HANDLE;
        VmaAllocation m_allocation = VK_NULL_HANDLE;
    };
}


#endif //EUGINE_VKVERTEXBUFFER_H

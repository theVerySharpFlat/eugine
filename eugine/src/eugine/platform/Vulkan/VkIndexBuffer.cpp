//
// Created by aiden on 6/29/22.
//

#include "VkIndexBuffer.h"

#include "VkBufferUtil.h"

namespace eg::rendering::VKWrapper {
    VkIndexBuffer::VkIndexBuffer(VkDevice& device, VkCommandPool commandPool, VmaAllocator& allocator, const u16* data,
                                 u32 count, VertexBuffer::UsageHints usageHint) : m_commandPool(commandPool),
                                                                                 m_allocator(allocator),
                                                                                 m_device(device),
                                                                                 m_count(count) {
        u32 size = sizeof(u16) * count;
        if (usageHint == VertexBuffer::VB_USAGE_HINT_DYNAMIC) {
            BufferUtil::createBuffer(m_allocator, VMA_MEMORY_USAGE_CPU_TO_GPU, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, size,
                                     &m_buffer, &m_allocation);

            void* mapped;
            vmaMapMemory(m_allocator, m_allocation, &mapped);
            memcpy(mapped, data, size);
            vmaUnmapMemory(m_allocator, m_allocation);
        } else if (usageHint == VertexBuffer::VB_USAGE_HINT_STATIC) {
            VkBuffer stagingBuffer;
            VmaAllocation stagingBufferAllocation;
            {
                BufferUtil::createBuffer(m_allocator, VMA_MEMORY_USAGE_CPU_ONLY, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, size,
                                         &stagingBuffer, &stagingBufferAllocation);
                void* mapped;
                vmaMapMemory(allocator, stagingBufferAllocation, &mapped);
                memcpy(mapped, data, size);
                vmaUnmapMemory(allocator, stagingBufferAllocation);
            }

            BufferUtil::createBuffer(m_allocator, VMA_MEMORY_USAGE_GPU_ONLY,
                                     VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, size,
                                     &m_buffer, &m_allocation);

            BufferUtil::copyBuffer(m_device, m_commandPool, stagingBuffer, m_buffer, size);

            vmaDestroyBuffer(allocator, stagingBuffer, stagingBufferAllocation);
        }
    }

    VkIndexBuffer::~VkIndexBuffer() {
        vmaDestroyBuffer(m_allocator, m_buffer, m_allocation);
    }
}
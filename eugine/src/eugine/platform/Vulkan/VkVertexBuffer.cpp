//
// Created by aiden on 6/28/22.
//

#include "VkVertexBuffer.h"

#include "VkBufferUtil.h"

namespace eg::rendering::VKWrapper {
    VkVertexBuffer::VkVertexBuffer(VkDevice& device, VkCommandPool commandPool, VmaAllocator& allocator, void* data, u32 size,
                                   VertexBuffer::UsageHints usageHint)
            : m_allocator(allocator), m_device(device), m_maxSize(size), m_usageHint(usageHint) {
        if (usageHint == VertexBuffer::VB_USAGE_HINT_DYNAMIC) {
            BufferUtil::createBuffer(m_allocator, VMA_MEMORY_USAGE_CPU_TO_GPU, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, size,
                                     &m_buffer, &m_allocation);

            void* mapped;
            vmaMapMemory(m_allocator, m_allocation, &mapped);
            memcpy(mapped, data, size);
            vmaUnmapMemory(m_allocator, m_allocation);
        } else if (usageHint == VertexBuffer::VB_USAGE_HINT_STATIC) {
            VkBuffer stagingBuffer = VK_NULL_HANDLE;
            VmaAllocation stagingBufferAllocation = VK_NULL_HANDLE;

            {
                BufferUtil::createBuffer(m_allocator, VMA_MEMORY_USAGE_CPU_ONLY, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                                         size, &stagingBuffer, &stagingBufferAllocation);

                void* mapped;
                vmaMapMemory(m_allocator, stagingBufferAllocation, &mapped);
                memcpy(mapped, data, size);
                vmaUnmapMemory(m_allocator, stagingBufferAllocation);
            }

            BufferUtil::createBuffer(m_allocator, VMA_MEMORY_USAGE_GPU_ONLY,
                                     VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, size,
                                     &m_buffer, &m_allocation);

            BufferUtil::copyBuffer(device, commandPool, stagingBuffer, m_buffer, size);

            vmaDestroyBuffer(m_allocator, stagingBuffer, stagingBufferAllocation);
        }
    }

    VkVertexBuffer::~VkVertexBuffer() {
        vmaDestroyBuffer(m_allocator, m_buffer, m_allocation);
    }
}
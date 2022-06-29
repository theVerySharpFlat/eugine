//
// Created by aiden on 6/28/22.
//

#include "VkVertexBuffer.h"

#include "VkBufferUtil.h"

namespace eg::rendering::VKWrapper {
    VkVertexBuffer::VkVertexBuffer(VkDevice& device, VkCommandPool commandPool, VmaAllocator& allocator, void* data, u32 size,
                                   VertexBuffer::UsageHints usageHint)
            : m_allocator(allocator) {
        if (usageHint == VertexBuffer::VB_USAGE_HINT_DYNAMIC) {
            /* VkBufferCreateInfo bufferCreateInfo{};
            bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bufferCreateInfo.size = size;
            bufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
            bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

            VmaAllocationCreateInfo allocInfo{};
            allocInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;

            if (vmaCreateBuffer(m_allocator, &bufferCreateInfo, &allocInfo, &m_buffer, &m_allocation, nullptr) !=
                VK_SUCCESS) {
                error("failed to create vertex buffer!!!");
                return;
            }*/

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
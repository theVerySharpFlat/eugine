//
// Created by aiden on 6/28/22.
//

#include "VkVertexBuffer.h"

#include "VkBufferUtil.h"

namespace eg::rendering::VKWrapper {
    VkVertexBuffer::VkVertexBuffer(VkDevice& device, VkCommandPool commandPool, VmaAllocator& allocator, void* data,
                                   u32 size,
                                   VertexBuffer::UsageHints usageHint)
            : m_allocator(allocator), m_device(device), m_maxSize(size), m_usageHint(usageHint) {
        if (usageHint == VertexBuffer::VB_USAGE_HINT_DYNAMIC) {
            BufferUtil::createBuffer(m_allocator, VMA_MEMORY_USAGE_AUTO,
                                     VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                                     BufferUtil::BUFFER_DYNAMIC, size,
                                     &m_buffer, &m_allocation, &m_allocationInfo);

            VkMemoryPropertyFlags memoryPropertyFlags;
            vmaGetAllocationMemoryProperties(allocator, m_allocation, &memoryPropertyFlags);

            if (memoryPropertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) {
                memcpy(m_allocationInfo.pMappedData, data, size);
            } else {
                // someday soon we will test this. Just pray it works first try.

                VkBuffer stagingBuffer;
                VmaAllocation stagingBufferAllocation;
                VmaAllocationInfo stagingBufferAllocationInfo;

                BufferUtil::createBuffer(allocator, VMA_MEMORY_USAGE_AUTO, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                                         BufferUtil::BUFFER_STAGING, size, &stagingBuffer, &stagingBufferAllocation,
                                         &stagingBufferAllocationInfo);

                memcpy(stagingBufferAllocationInfo.pMappedData, data, size);

                BufferUtil::copyBuffer(device, commandPool, stagingBuffer, m_buffer, size);

                vmaDestroyBuffer(allocator, stagingBuffer, stagingBufferAllocation);
            }

        } else if (usageHint == VertexBuffer::VB_USAGE_HINT_STATIC) {
            VkBuffer stagingBuffer = VK_NULL_HANDLE;
            VmaAllocation stagingBufferAllocation = VK_NULL_HANDLE;
            VmaAllocationInfo stagingBufferAllocationInfo;

            {
                BufferUtil::createBuffer(m_allocator, VMA_MEMORY_USAGE_AUTO,
                                         VK_BUFFER_USAGE_TRANSFER_SRC_BIT, BufferUtil::BUFFER_STAGING,
                                         size, &stagingBuffer, &stagingBufferAllocation, &stagingBufferAllocationInfo);

                memcpy(stagingBufferAllocationInfo.pMappedData, data, size);
            }

            BufferUtil::createBuffer(m_allocator, VMA_MEMORY_USAGE_AUTO,
                                     VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                                     BufferUtil::BUFFER_STATIC, size,
                                     &m_buffer, &m_allocation);

            BufferUtil::copyBuffer(device, commandPool, stagingBuffer, m_buffer, size);

            vmaDestroyBuffer(m_allocator, stagingBuffer, stagingBufferAllocation);
        }
    }

    VkVertexBuffer::~VkVertexBuffer() {
        vmaDestroyBuffer(m_allocator, m_buffer, m_allocation);
    }
}
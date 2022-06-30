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
            BufferUtil::createBuffer(m_allocator, VMA_MEMORY_USAGE_AUTO,
                                     VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                                     BufferUtil::BUFFER_DYNAMIC, size,
                                     &m_buffer, &m_allocation, &m_allocationInfo);

            VkMemoryPropertyFlags memoryPropertyFlags;
            vmaGetAllocationMemoryProperties(allocator, m_allocation, &memoryPropertyFlags);

            if (memoryPropertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) {
                memcpy(m_allocationInfo.pMappedData, (const void*) data, size);
            } else {
                VkBuffer stagingBuffer;
                VmaAllocation stagingBufferAllocation;
                VmaAllocationInfo stagingBufferAllocationInfo;

                BufferUtil::createBuffer(allocator, VMA_MEMORY_USAGE_AUTO, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                                         BufferUtil::BUFFER_STAGING, size, &stagingBuffer, &stagingBufferAllocation,
                                         &stagingBufferAllocationInfo);

                memcpy(stagingBufferAllocationInfo.pMappedData, (const void*) data, size);

                BufferUtil::copyBuffer(device, commandPool, stagingBuffer, m_buffer, size);

                vmaDestroyBuffer(allocator, stagingBuffer, stagingBufferAllocation);
            }

        } else if (usageHint == VertexBuffer::VB_USAGE_HINT_STATIC) {
            VkBuffer stagingBuffer;
            VmaAllocation stagingBufferAllocation;
            {
                VmaAllocationInfo stagingBufferAllocationInfo;

                BufferUtil::createBuffer(m_allocator, VMA_MEMORY_USAGE_AUTO_PREFER_HOST,
                                         VK_BUFFER_USAGE_TRANSFER_SRC_BIT, BufferUtil::BUFFER_STAGING, size,
                                         &stagingBuffer, &stagingBufferAllocation, &stagingBufferAllocationInfo);

                memcpy(stagingBufferAllocationInfo.pMappedData, (const void*) data, size);
            }

            BufferUtil::createBuffer(m_allocator, VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE,
                                     VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, BufferUtil::BUFFER_STATIC, size,
                                     &m_buffer, &m_allocation);

            BufferUtil::copyBuffer(m_device, m_commandPool, stagingBuffer, m_buffer, size);

            vmaDestroyBuffer(allocator, stagingBuffer, stagingBufferAllocation);
        }
    }

    VkIndexBuffer::~VkIndexBuffer() {
        vmaDestroyBuffer(m_allocator, m_buffer, m_allocation);
    }
}
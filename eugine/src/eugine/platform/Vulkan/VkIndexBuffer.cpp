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
                                                                                  m_usageHint(usageHint),
                                                                                  m_maxCount(count),
                                                                                  m_count(count) {
        u32 size = sizeof(u16) * count;
        if (usageHint == VertexBuffer::VB_USAGE_HINT_DYNAMIC) {
            BufferUtil::createBuffer(m_allocator, VMA_MEMORY_USAGE_AUTO,
                                     VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                                     BufferUtil::BUFFER_DYNAMIC, size,
                                     &m_buffer, &m_allocation, &m_allocationInfo);

        } else if (usageHint == VertexBuffer::VB_USAGE_HINT_STATIC) {
            BufferUtil::createBuffer(m_allocator, VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE,
                                     VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, BufferUtil::BUFFER_STATIC, size,
                                     &m_buffer, &m_allocation);
        }

        if(data != nullptr) {
            setData(data, count);
        }
    }

    void VkIndexBuffer::setData(const u16* data, u32 count) {
        if(count > m_maxCount) {
            error("failed to update index buffer: count is above maximum");
            return;
        }
        m_count = count;

        u32 size = sizeof(u16) * count;
        if (m_usageHint == VertexBuffer::VB_USAGE_HINT_DYNAMIC) {
            VkMemoryPropertyFlags memoryPropertyFlags;
            vmaGetAllocationMemoryProperties(m_allocator, m_allocation, &memoryPropertyFlags);

            if (memoryPropertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) {
                memcpy(m_allocationInfo.pMappedData, data, size);
            } else {
                // someday soon we will test this. Just pray it works first try.

                VkBuffer stagingBuffer;
                VmaAllocation stagingBufferAllocation;
                VmaAllocationInfo stagingBufferAllocationInfo;

                BufferUtil::createBuffer(m_allocator, VMA_MEMORY_USAGE_AUTO, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                                         BufferUtil::BUFFER_STAGING, size, &stagingBuffer, &stagingBufferAllocation,
                                         &stagingBufferAllocationInfo);

                memcpy(stagingBufferAllocationInfo.pMappedData, data, size);

                BufferUtil::copyBuffer(m_device, m_commandPool, stagingBuffer, m_buffer, size);

                vmaDestroyBuffer(m_allocator, stagingBuffer, stagingBufferAllocation);
            }
        } else if (m_usageHint == VertexBuffer::VB_USAGE_HINT_STATIC) {
            VkBuffer stagingBuffer = VK_NULL_HANDLE;
            VmaAllocation stagingBufferAllocation = VK_NULL_HANDLE;
            VmaAllocationInfo stagingBufferAllocationInfo;

            BufferUtil::createBuffer(m_allocator, VMA_MEMORY_USAGE_AUTO,
                                     VK_BUFFER_USAGE_TRANSFER_SRC_BIT, BufferUtil::BUFFER_STAGING,
                                     size, &stagingBuffer, &stagingBufferAllocation, &stagingBufferAllocationInfo);

            memcpy(stagingBufferAllocationInfo.pMappedData, data, size);

            BufferUtil::copyBuffer(m_device, m_commandPool, stagingBuffer, m_buffer, size);

            vmaDestroyBuffer(m_allocator, stagingBuffer, stagingBufferAllocation);
        }
    }

    void VkIndexBuffer::free() {
        if(m_buffer != VK_NULL_HANDLE)
            vmaDestroyBuffer(m_allocator, m_buffer, m_allocation);
    }

    VkIndexBuffer::~VkIndexBuffer() {
        free();
    }
}
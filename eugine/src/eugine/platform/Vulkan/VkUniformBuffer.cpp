//
// Created by aiden on 7/17/22.
//

#include "VkUniformBuffer.h"
#include "VkBufferUtil.h"

namespace eg::rendering::VKWrapper {
    VkUniformBuffer::VkUniformBuffer(VkDevice& device, VkCommandPool commandPool, VmaAllocator& allocator, void* data,
                                     u32 size, VertexBuffer::UsageHints usageHint)
        : m_allocator(allocator), m_device(device), m_commandPool(commandPool), m_maxSize(size), m_currentSize(size),
                m_usageHint(usageHint) {
            if (usageHint == VertexBuffer::VB_USAGE_HINT_DYNAMIC) {
            BufferUtil::createBuffer(m_allocator, VMA_MEMORY_USAGE_AUTO,
                                     VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
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
                                     VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                                     BufferUtil::BUFFER_STATIC, size,
                                     &m_buffer, &m_allocation);

            BufferUtil::copyBuffer(device, commandPool, stagingBuffer, m_buffer, size);

            vmaDestroyBuffer(m_allocator, stagingBuffer, stagingBufferAllocation);
        }
    }

    void VkUniformBuffer::setData(void* data, u32 size) {
        if(size > m_maxSize) {
            error("failed to update vertex buffer: size is greater than maxSize");
            return;
        }
        m_currentSize = size;

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

    VkUniformBuffer::~VkUniformBuffer() {
        vmaDestroyBuffer(m_allocator, m_buffer, m_allocation);
    }
}

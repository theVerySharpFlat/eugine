//
// Created by aiden on 6/28/22.
//

#include "VkVertexBuffer.h"

#include "VkBufferUtil.h"

namespace eg::rendering::VKWrapper {
    VkVertexBuffer::VkVertexBuffer(VkDevice& device, VkCommandPool commandPool, VmaAllocator& allocator, void* data,
                                   u32 size, VertexBuffer::UsageHints usageHint, VertexBufferLayout& layout)
            : m_allocator(allocator),
              m_device(device),
              m_commandPool(commandPool),
              m_maxSize(size), m_size(m_maxSize),
              m_usageHint(usageHint),
              m_layout(layout) {
        if (usageHint == VertexBuffer::VB_USAGE_HINT_DYNAMIC) {
            BufferUtil::createBuffer(m_allocator, VMA_MEMORY_USAGE_AUTO,
                                     VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                                     BufferUtil::BUFFER_DYNAMIC, size,
                                     &m_buffer, &m_allocation, &m_allocationInfo);


        } else if (usageHint == VertexBuffer::VB_USAGE_HINT_STATIC) {
            BufferUtil::createBuffer(m_allocator, VMA_MEMORY_USAGE_AUTO,
                                     VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                                     BufferUtil::BUFFER_STATIC, size,
                                     &m_buffer, &m_allocation);
        }

        if (data != nullptr) {
            _setData(data, size);
        }
    }

    void VkVertexBuffer::setData(void* data, u32 size) {
        _setData(data, size);
    }

    void VkVertexBuffer::_setData(void* data, u32 size) {
        if (size > m_maxSize) {
            error("failed to update vertex buffer: size is greater than maxSize");
            return;
        }
        m_size = size;

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

    void VkVertexBuffer::free() {
        if (m_buffer != VK_NULL_HANDLE)
            vmaDestroyBuffer(m_allocator, m_buffer, m_allocation);
    }

    VkVertexBuffer::~VkVertexBuffer() {
        free();
    }

    void VkVertexBuffer::setLayout(const VertexBufferLayout& layout) {
        m_layout = layout;
    }

    const VertexBufferLayout& VkVertexBuffer::getLayout() const {
        return m_layout;
    }
}
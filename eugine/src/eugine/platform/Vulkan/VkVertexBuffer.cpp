//
// Created by aiden on 6/28/22.
//

#include "VkVertexBuffer.h"

namespace eg::rendering::VKWrapper {
    VkVertexBuffer::VkVertexBuffer(VkDevice& device, VmaAllocator& allocator, void* data, u32 size)
            : m_allocator(allocator) {
        VkBufferCreateInfo bufferCreateInfo{};
        bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferCreateInfo.size = size;
        bufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VmaAllocationCreateInfo allocInfo{};
        allocInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;

        if(vmaCreateBuffer(m_allocator, &bufferCreateInfo, &allocInfo, &m_buffer, &m_allocation, nullptr) != VK_SUCCESS) {
            error("failed to create vertex buffer!!!");
            return;
        }

        void* mapped;
        vmaMapMemory(m_allocator, m_allocation, &mapped);
        memcpy(mapped, data, size);
        vmaUnmapMemory(m_allocator, m_allocation);
    }

    VkVertexBuffer::~VkVertexBuffer() {
        vmaDestroyBuffer(m_allocator, m_buffer, m_allocation);
    }
}
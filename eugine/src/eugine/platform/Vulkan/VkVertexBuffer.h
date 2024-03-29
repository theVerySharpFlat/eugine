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

    class VkVertexBuffer : public ::eg::rendering::VertexBuffer {
    public:
        VkVertexBuffer(VkDevice& device, VkCommandPool commandPool, VmaAllocator& allocator, void* data, u32 size,
                       VertexBuffer::UsageHints usageHint, VertexBufferLayout& layout);

        ~VkVertexBuffer();

        void setData(void* data, u32 size) override;
        void _setData(void* data, u32 size);

        VkBuffer& getBuffer() { return m_buffer; }

        u32 getSize() { return m_size; }
        u32 getMaxSize() { return m_maxSize; }

        void free();

        VkVertexBuffer& operator=(const VkVertexBuffer& vertexBuffer) {
            return *this;
        }

        void setLayout(const VertexBufferLayout& layout) override;
        const VertexBufferLayout& getLayout() const override;

    private:
        VmaAllocator& m_allocator;
        VkDevice& m_device;

        VkCommandPool m_commandPool;

        u32 m_maxSize;
        u32 m_size;
        VertexBuffer::UsageHints m_usageHint;

        VkBuffer m_buffer = VK_NULL_HANDLE;
        VmaAllocation m_allocation = VK_NULL_HANDLE;
        VmaAllocationInfo m_allocationInfo{};

        VertexBufferLayout& m_layout;
    };
}


#endif //EUGINE_VKVERTEXBUFFER_H

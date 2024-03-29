//
// Created by aiden on 6/29/22.
//

#ifndef EUGINE_VKINDEXBUFFER_H
#define EUGINE_VKINDEXBUFFER_H

#include <volk.h>
#include "vk_mem_alloc.h"

#include "eugine/rendering/VertexBuffer.h"
#include "eugine/rendering/IndexBuffer.h"

namespace eg::rendering::VKWrapper {
    class VkDevice;

    class VkIndexBuffer : public ::eg::rendering::IndexBuffer {
    public:
        VkIndexBuffer(VkDevice& device, VkCommandPool commandPool, VmaAllocator& allocator, const u16* data, u32 count,
                      VertexBuffer::UsageHints usageHint);

        ~VkIndexBuffer();

        void setData(const u16* data, u32 count) override;

        void free();

        VkBuffer getBuffer() { return m_buffer; }

        u32 getCount() const { return m_count; }
        u32 getElementCount() const override { return getCount(); }
        u32 getMaxElementCount() const override { return m_maxCount; }

        VkIndexBuffer& operator=(const VkIndexBuffer& ibo) {
            return *this;
        }

    private:
        VkBuffer m_buffer = VK_NULL_HANDLE;
        VkCommandPool m_commandPool;
        VmaAllocation m_allocation = VK_NULL_HANDLE;
        VmaAllocator m_allocator;
        VmaAllocationInfo m_allocationInfo{};
        VkDevice& m_device;

        VertexBuffer::UsageHints m_usageHint;
        u32 m_maxCount;
        u32 m_count;
    };
}


#endif //EUGINE_VKINDEXBUFFER_H

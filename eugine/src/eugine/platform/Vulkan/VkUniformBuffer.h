//
// Created by aiden on 7/17/22.
//

#ifndef EUGINE_VKUNIFORMBUFFER_H
#define EUGINE_VKUNIFORMBUFFER_H

#include "volk.h"
#include "vk_mem_alloc.h"

#include "eugine/rendering/VertexBuffer.h"
#include "eugine/rendering/UniformBuffer.h"

namespace eg::rendering::VKWrapper {
    class VkDevice;

    class VkUniformBuffer : public rendering::UniformBuffer {
    public:
        VkUniformBuffer(VkDevice& device, VkCommandPool commandPool, VmaAllocator& allocator, void* data, u32 size,
                        VertexBuffer::UsageHints usageHint);

        ~VkUniformBuffer() override;

        void setData(void* data, u32 size) override;

        void free();

        VkBuffer& getBuffer() { return m_buffer; }

        u32 getSize() { return m_currentSize; }

        VkUniformBuffer& operator=(const VkUniformBuffer& ubo) {
            return *this;
        }

    private:
        VmaAllocator& m_allocator;
        VkDevice& m_device;

        VkCommandPool m_commandPool;

        u32 m_maxSize;
        u32 m_currentSize;
        VertexBuffer::UsageHints m_usageHint;

        VkBuffer m_buffer = VK_NULL_HANDLE;
        VmaAllocation m_allocation = VK_NULL_HANDLE;
        VmaAllocationInfo m_allocationInfo{};
    };
}


#endif //EUGINE_VKUNIFORMBUFFER_H

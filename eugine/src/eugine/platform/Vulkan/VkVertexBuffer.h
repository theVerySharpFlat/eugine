//
// Created by aiden on 6/28/22.
//

#ifndef EUGINE_VKVERTEXBUFFER_H
#define EUGINE_VKVERTEXBUFFER_H

#include <volk.h>
#include <vk_mem_alloc.h>

namespace eg::rendering::VKWrapper {
    class VkDevice;
    class VkVertexBuffer {
    public:
        VkVertexBuffer(VkDevice& device, VmaAllocator& allocator, void* data, u32 size);
        ~VkVertexBuffer();

        VkBuffer& getBuffer() { return m_buffer; }

    private:
        VmaAllocator& m_allocator;

        VkBuffer m_buffer = VK_NULL_HANDLE;
        VmaAllocation m_allocation = VK_NULL_HANDLE;
    };
}


#endif //EUGINE_VKVERTEXBUFFER_H

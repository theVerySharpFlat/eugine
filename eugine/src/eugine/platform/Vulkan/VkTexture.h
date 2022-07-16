//
// Created by aiden on 7/15/22.
//

#ifndef EUGINE_VKTEXTURE_H
#define EUGINE_VKTEXTURE_H

#include "vk_mem_alloc.h"
#include "volk.h"

namespace eg::rendering::VKWrapper {
    class VkDevice;

    class VkTexture {
    public:
        VkTexture(VkDevice& device, VmaAllocator& allocator);
        ~VkTexture();

        void init(const char* path);
        void destruct();

    private:
        VmaAllocator& m_allocator;
        VkDevice& m_device;

        VkImage m_image = VK_NULL_HANDLE;
        VmaAllocation m_imageAllocation = VK_NULL_HANDLE;
        VmaAllocationInfo m_imageAllocationInfo{};

        u32 m_width = 0, m_height = 0, m_numChannels = 0;
    };
}

#endif //EUGINE_VKTEXTURE_H

//
// Created by aiden on 7/15/22.
//

#ifndef EUGINE_VKTEXTURE_H
#define EUGINE_VKTEXTURE_H

#include "volk.h"
#include "vk_mem_alloc.h"

#include "eugine/rendering/Texture.h"

namespace eg::rendering::VKWrapper {
    class VkDevice;

    class VkTexture : public eg::rendering::Texture {
    public:
        VkTexture(VkDevice& device, VmaAllocator& allocator, VkCommandPool& commandPool);

        ~VkTexture();

        void init(const char* path);

        void destruct();

        VkImageView getImageView() { return m_imageView; }

        VkSampler getSampler() { return m_sampler; }

        static void
        transitionImageLayout(VkCommandBuffer commandBuffer, VkImage image, VkFormat format, VkImageLayout oldLayout,
                              VkImageLayout newLayout);

        static void
        copyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer buffer, VkImage image, u32 width, u32 height);

        bool operator==(const Texture& texture) const override;

    private:
        VmaAllocator& m_allocator;
        VkDevice& m_device;
        VkCommandPool& m_commandPool;

        VkImage m_image = VK_NULL_HANDLE;
        VkImageView m_imageView = VK_NULL_HANDLE;
        VkSampler m_sampler = VK_NULL_HANDLE;
        VmaAllocation m_imageAllocation = VK_NULL_HANDLE;
        VmaAllocationInfo m_imageAllocationInfo{};

        u32 m_width = 0, m_height = 0, m_numChannels = 0;
    };
}

#endif //EUGINE_VKTEXTURE_H

//
// Created by aiden on 8/19/22.
//

#ifndef EUGINE_VKOFFSCREENSUBRENDERER_H
#define EUGINE_VKOFFSCREENSUBRENDERER_H

#include "volk.h"
#include "vk_mem_alloc.h"
#include "VkRenderPass.h"

namespace eg::rendering::VKWrapper {
    class VkDevice;
    class VkWindow;

    class VkOffscreenSubRenderer {
    public:
        VkOffscreenSubRenderer(VkDevice& device, VkWindow& window, VmaAllocator& allocator);
        ~VkOffscreenSubRenderer();

        void init();
        void onWindowResize();
        void destruct();

        void begin(VkCommandBuffer commandBuffer, u32 imageIndex);
        void end(VkCommandBuffer commandBuffer, u32 imageIndex);

        VkRenderPass& getRenderPass() { return m_renderPass; };

    private:
        VkDevice& m_device;
        VkWindow& m_window;
        VmaAllocator& m_allocator;

        VkRenderPass m_renderPass{m_device, m_window};

        struct ImageData {
            VkImage image = VK_NULL_HANDLE;
            VkImageView imageView = VK_NULL_HANDLE;
            VmaAllocation allocation = VK_NULL_HANDLE;
            VmaAllocationInfo allocationInfo{};
        };

        std::vector<ImageData> m_images;
        VkSampler m_sampler;
        std::vector<VkFramebuffer> m_frameBuffers;
    };
}


#endif //EUGINE_VKOFFSCREENSUBRENDERER_H

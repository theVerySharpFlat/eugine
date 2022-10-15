//
// Created by aiden on 10/8/22.
//

#ifndef EUGINE_VKFRAMEBUFFER_H
#define EUGINE_VKFRAMEBUFFER_H

#include "eugine/platform/Vulkan/VkImguiSystem.h"
#include <volk.h>
#include <eugine/rendering/Framebuffer.h>
#include <vk_mem_alloc.h>
#include <vulkan/vulkan_core.h>

namespace eg::rendering::VKWrapper {
    class VKAPI;
    class VkDevice;
    class VkWindow;
    class VkRenderPass;
    class VkFramebuffer : public Framebuffer {
    public:
        VkFramebuffer(VKAPI& api, VkDevice& device, VkWindow& window);
        ~VkFramebuffer() override { destruct(); };

        void init(VkRenderPass& renderPass, Usage usage, VmaAllocator allocator);
        void init(VkRenderPass& renderPass, VkSwapchainKHR swapchain, VkFormat imageFormat);
        void destruct();

        Usage getUsage() { return m_usage; }

        ::VkFramebuffer& getFramebuffer(u32 index) { return m_framebuffers[index]; }

        static void createGlobalSampler(VkDevice& device);
        static void destroyGlobalSampler(VkDevice& device);

    private:
        friend class VkImguiSystem;
        VKAPI& m_api;
        VkDevice& m_device;
        VkWindow& m_window;

        struct ImageData {
            VkImage image = VK_NULL_HANDLE;
            VkImageView imageView = VK_NULL_HANDLE;
            VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
            VmaAllocation allocation = VK_NULL_HANDLE;
            VmaAllocationInfo allocationInfo{};
        };
        VmaAllocator m_allocator = VK_NULL_HANDLE;
        std::vector<ImageData> m_images;

        std::vector<::VkFramebuffer> m_framebuffers;
        void createFramebuffers(VkRenderPass& renderPass);

        Usage m_usage;

        bool m_initSuccess = true;

        static VkSampler sampler;
    };
}


#endif //EUGINE_VKFRAMEBUFFER_H

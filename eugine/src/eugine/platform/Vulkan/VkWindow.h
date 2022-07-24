//
// Created by aiden on 6/1/22.
//

#ifndef EUGINE_VKWINDOW_H
#define EUGINE_VKWINDOW_H

#include <volk.h>
#include <eugine/core/window.h>

namespace eg::rendering::VKWrapper {
    class VKAPI;
    class VkDevice;
    class VkWindow;
    class VkRenderPass;

    class VkWindow {
    public:
        VkWindow(VKAPI& vkapi, VkDevice& device, VkRenderPass& renderPass, Window& window);

        void initialize();
        void destruct();

        void createSwapchain();
        void destroySwapchain();
        ::VkSwapchainKHR& getSwapchain() { return m_swapchain; }

        void createFrameBuffers();
        void destroyFrameBuffers();
        ::VkFramebuffer& getFrameBuffer(u32 index) { return m_framebuffers[index]; }

        void updateSwapchainExtent();
        VkExtent2D getSwapchainExtent() { return m_swapchainExtent; }

        ::VkFormat& getSwapchainImageFormat() { return m_swapchainImageFormat; }

        ~VkWindow();

        VkSurfaceKHR& getSurface() { return m_surface; }

        u32 getImageCount() const { return m_imageCount; }
        u32 getMinImageCount() const { return m_minImageCount; }

    private:
        friend class VKAPI;
        friend class VkRenderPass;
        friend class VkShader;


        VkDevice& m_device;
        VkInstance& m_instance;
        VkRenderPass& m_renderpass;

        VkSurfaceFormatKHR chooseSurfaceFormat(VkSurfaceFormatKHR* formats, u32 count);
        VkPresentModeKHR   choosePresentMode(VkPresentModeKHR* presentModes, u32 count);
        VkExtent2D         chooseSwapExtent(VkSurfaceCapabilitiesKHR capabilities);

        void* m_window;

        VkSurfaceKHR m_surface = VK_NULL_HANDLE;
        VkSwapchainKHR m_swapchain = VK_NULL_HANDLE;;

        VkFormat m_swapchainImageFormat;
        VkExtent2D m_swapchainExtent;

        std::vector<VkImage> m_swapchainImages;
        std::vector<VkImageView> m_swapchainImageViews;

        u32 m_minImageCount = 0;
        u32 m_imageCount = 0;

        std::vector<VkFramebuffer> m_framebuffers;

        void createSurface();
    };
}


#endif //EUGINE_VKWINDOW_H

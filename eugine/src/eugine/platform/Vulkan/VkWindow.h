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

    class VkWindow {
    public:
        VkWindow(VKAPI& vkapi, VkDevice& device, Window& window);

        void initialize();
        void destruct();

        void createSwapchain();
        void destroySwapchain();

        ~VkWindow();

        VkSurfaceKHR& getSurface() { return m_surface; }

    private:
        friend class VkRenderPass;
        friend class VkShader;

        VkDevice& m_device;
        VkInstance& m_instance;

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

        void createSurface();
    };
}


#endif //EUGINE_VKWINDOW_H

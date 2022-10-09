//
// Created by aiden on 6/1/22.
//

#include "VkWindow.h"
#include "VKAPI.h"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

namespace eg::rendering::VKWrapper {
    VkWindow::VkWindow(VKAPI& vkapi, VkDevice& device, VkRenderPass& renderPass, Window& window) : m_instance(vkapi.m_instance),
                                                                         m_device(device),
                                                                         m_renderpass(renderPass),
                                                                         m_window(window.getNativeWindow()),
                                                                         m_frameBuffer(vkapi, device, *this){}

    void VkWindow::initialize() {
        createSurface();
    }

    void VkWindow::destruct() {
        if(m_surface != VK_NULL_HANDLE)
            vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
    }

    VkSurfaceFormatKHR VkWindow::chooseSurfaceFormat(VkSurfaceFormatKHR* formats, u32 count) {
        for (int i = 0; i < count; i++) {
            if (formats[i].format == VK_FORMAT_B8G8R8A8_UNORM &&
                formats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                // trace("going with preferred surface format");
                return formats[i];
            }
        }
        return formats[0];
    }

    VkPresentModeKHR VkWindow::choosePresentMode(VkPresentModeKHR* presentModes, u32 count) {
        for (int i = 0; i < count; i++) {
            if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
                // trace("going with preferred");
                return presentModes[i];
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D VkWindow::chooseSwapExtent(VkSurfaceCapabilitiesKHR capabilities) {
        i32 width = 0, height = 0;
        glfwGetFramebufferSize((GLFWwindow*) m_window, &width, &height);
        VkExtent2D windowExtent = {(u32) width, (u32) height};

        if (capabilities.currentExtent.width != UINT32_MAX) {
            return capabilities.currentExtent;
        } else {
            VkExtent2D extent;
            extent.height = std::clamp(windowExtent.height, capabilities.minImageExtent.height,
                                       capabilities.maxImageExtent.height);
            extent.width = std::clamp(windowExtent.width, capabilities.minImageExtent.width,
                                      capabilities.maxImageExtent.width);
            return extent;
        }
    }

    void VkWindow::updateSwapchainExtent() {
        VkSurfaceCapabilitiesKHR surfaceCapabilities;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_device.getPhysicalDevice(), m_surface, &surfaceCapabilities);

        m_swapchainExtent = chooseSwapExtent(surfaceCapabilities);
    }

    u32 VkWindow::getSwapchainImageCount() const {
        u32 count;
        vkGetSwapchainImagesKHR(m_device.getDevice(), m_swapchain, &count, nullptr);
        return count;
    }

    ::VkFramebuffer& VkWindow::getFrameBuffer(u32 index) {
        return m_frameBuffer.getFramebuffer(index);
    }

    void VkWindow::createSwapchain() {
        VkSurfaceCapabilitiesKHR surfaceCapabilities;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_device.getPhysicalDevice(), m_surface, &surfaceCapabilities);

        u32 surfaceFormatCount = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(m_device.getPhysicalDevice(), m_surface, &surfaceFormatCount, nullptr);
        if (surfaceFormatCount == 0) {
            error("failed to retrieve vulkan surface formats!!!");
            m_initSuccess = false;
            return;
        }
        auto surfaceFormats = (VkSurfaceFormatKHR*) alloca(sizeof(VkSurfaceFormatKHR) * surfaceFormatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(m_device.getPhysicalDevice(), m_surface, &surfaceFormatCount, surfaceFormats);

        u32 presentModeCount = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(m_device.getPhysicalDevice(), m_surface, &presentModeCount, nullptr);
        if (presentModeCount == 0) {
            error("failed to retrieve vulkan present modes!!!");
            m_initSuccess = false;
            return;
        }
        auto presentModes = (VkPresentModeKHR*) alloca(sizeof(VkPresentModeKHR) * presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(m_device.getPhysicalDevice(), m_surface, &presentModeCount, presentModes);

        VkExtent2D extent = chooseSwapExtent(surfaceCapabilities);
        VkPresentModeKHR presentMode = choosePresentMode(presentModes, presentModeCount);
        VkSurfaceFormatKHR surfaceFormat = chooseSurfaceFormat(surfaceFormats, surfaceFormatCount);

        u32 imageCount = surfaceCapabilities.minImageCount + 1;
        if (surfaceCapabilities.maxImageCount > 0 && imageCount > surfaceCapabilities.maxImageCount) {
            imageCount = surfaceCapabilities.maxImageCount;
        }

        m_minImageCount = surfaceCapabilities.minImageCount;
        m_imageCount = imageCount;

        auto queueFamilyIndices = m_device.getQueueFamilyIndices();
        const u32 indicesCount = 2;
        u32 indicesArray[indicesCount] = {queueFamilyIndices.graphicsFamily.value(), queueFamilyIndices.presentFamily.value()};

        VkSwapchainCreateInfoKHR swapchainCreateInfo{};
        swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        swapchainCreateInfo.surface = m_surface;
        swapchainCreateInfo.clipped = VK_TRUE;
        swapchainCreateInfo.imageArrayLayers = 1;
        swapchainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
        swapchainCreateInfo.imageFormat = surfaceFormat.format;
        swapchainCreateInfo.imageExtent = extent;
        swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        swapchainCreateInfo.minImageCount = imageCount;
        if(queueFamilyIndices.presentFamily.value() != queueFamilyIndices.graphicsFamily.value()) {
            swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            swapchainCreateInfo.queueFamilyIndexCount = indicesCount;
            swapchainCreateInfo.pQueueFamilyIndices = indicesArray;
        } else {
            swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            swapchainCreateInfo.queueFamilyIndexCount = 0;
            swapchainCreateInfo.pQueueFamilyIndices = nullptr;
        }
        swapchainCreateInfo.preTransform = surfaceCapabilities.currentTransform;
        swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        swapchainCreateInfo.presentMode = presentMode;
        swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

        if(vkCreateSwapchainKHR(m_device.getDevice(), &swapchainCreateInfo, nullptr, &m_swapchain) != VK_SUCCESS) {
            error("failed to create vulkan swapchain!");
            m_initSuccess = false;
            return;
        }

        m_swapchainImageFormat = surfaceFormat.format;
        m_swapchainExtent = extent;

        u32 swapchainImageCount;
        vkGetSwapchainImagesKHR(m_device.getDevice(), m_swapchain, &swapchainImageCount, nullptr);
        if(!swapchainImageCount) {
            error("failed to retrieve vulkan swapchain images!");
            m_initSuccess = false;
            return;
        }
    }

    void VkWindow::destroySwapchain() {
        if(m_swapchain != VK_NULL_HANDLE)
            vkDestroySwapchainKHR(m_device.getDevice(), m_swapchain, nullptr);
    }

    VkWindow::~VkWindow() {

    }

    void VkWindow::createFrameBuffers() {
        m_frameBuffer.init(m_renderpass, m_swapchain, m_swapchainImageFormat);
    }

    void VkWindow::destroyFrameBuffers() {
        m_frameBuffer.destruct();
    }

    void VkWindow::createSurface() {
        if (glfwCreateWindowSurface(m_instance, (GLFWwindow*) m_window, nullptr, &m_surface) != VK_SUCCESS) {
            eg::error("failed to create vulkan surface!!!");
            m_initSuccess = false;
            return;
        }
    }
}

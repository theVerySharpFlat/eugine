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
                                                                         m_window(window.getNativeWindow()) {}

    void VkWindow::initialize() {
        createSurface();
    }

    void VkWindow::destruct() {
        vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
    }

    VkSurfaceFormatKHR VkWindow::chooseSurfaceFormat(VkSurfaceFormatKHR* formats, u32 count) {
        for (int i = 0; i < count; i++) {
            if (formats[i].format == VK_FORMAT_B8G8R8A8_SRGB &&
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

    void VkWindow::createSwapchain() {
        VkSurfaceCapabilitiesKHR surfaceCapabilities;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_device.getPhysicalDevice(), m_surface, &surfaceCapabilities);

        u32 surfaceFormatCount = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(m_device.getPhysicalDevice(), m_surface, &surfaceFormatCount, nullptr);
        if (surfaceFormatCount == 0) {
            fatal("failed to retrieve surface formats!!!");
            return;
        }
        auto surfaceFormats = (VkSurfaceFormatKHR*) alloca(sizeof(VkSurfaceFormatKHR) * surfaceFormatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(m_device.getPhysicalDevice(), m_surface, &surfaceFormatCount, surfaceFormats);

        u32 presentModeCount = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(m_device.getPhysicalDevice(), m_surface, &presentModeCount, nullptr);
        if (presentModeCount == 0) {
            fatal("failed to retrieve present modes!!!");
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
            fatal("failed to create swapchain!");
            return;
        }

        m_swapchainImageFormat = surfaceFormat.format;
        m_swapchainExtent = extent;

        u32 swapchainImageCount;
        vkGetSwapchainImagesKHR(m_device.getDevice(), m_swapchain, &swapchainImageCount, nullptr);
        if(!swapchainImageCount) {
            fatal("failed to retrieve swapchain images!");
            return;
        }
        m_swapchainImages.resize(swapchainImageCount);
        vkGetSwapchainImagesKHR(m_device.getDevice(), m_swapchain, &swapchainImageCount, m_swapchainImages.data());

        m_swapchainImageViews.resize(m_swapchainImages.size());
        for(u32 i = 0; i < m_swapchainImages.size(); i++) {
            VkImageViewCreateInfo imageViewCreateInfo{};
            imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            imageViewCreateInfo.image = m_swapchainImages[i];
            imageViewCreateInfo.format = m_swapchainImageFormat;
            imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
            imageViewCreateInfo.subresourceRange.levelCount = 1;
            imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
            imageViewCreateInfo.subresourceRange.layerCount = 1;
            
            if(vkCreateImageView(m_device.getDevice(), &imageViewCreateInfo, nullptr, &m_swapchainImageViews[i]) != VK_SUCCESS) {
                fatal("failed to create swapchain image view {}", i);
                return;
            }
        }
    }

    void VkWindow::destroySwapchain() {
        for(auto & m_swapchainImageView : m_swapchainImageViews) {
            vkDestroyImageView(m_device.getDevice(), m_swapchainImageView, nullptr);
        }
        vkDestroySwapchainKHR(m_device.getDevice(), m_swapchain, nullptr);
    }

    VkWindow::~VkWindow() {

    }

    void VkWindow::createFrameBuffers() {
        m_framebuffers.resize(m_swapchainImageViews.size());

        for (int i = 0; i < m_swapchainImages.size(); i++) {
            VkImageView attachments[] = {
                    m_swapchainImageViews[i]
            };

            VkFramebufferCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            createInfo.renderPass = m_renderpass.getRenderPass();
            createInfo.attachmentCount = 1;
            createInfo.pAttachments = attachments;
            createInfo.width = m_swapchainExtent.width;
            createInfo.height = m_swapchainExtent.height;
            createInfo.layers = 1;

            if(vkCreateFramebuffer(m_device.getDevice(), &createInfo, nullptr, &m_framebuffers[i]) != VK_SUCCESS) {
                error("failed to create framebuffer {}!!!", i);
                return;
            }
        }
    }

    void VkWindow::destroyFrameBuffers() {
        for(u32 i = 0; i < m_framebuffers.size(); i++) {
            vkDestroyFramebuffer(m_device.getDevice(), m_framebuffers[i], nullptr);
        }
    }

    void VkWindow::createSurface() {
        if (glfwCreateWindowSurface(m_instance, (GLFWwindow*) m_window, nullptr, &m_surface) != VK_SUCCESS) {
            eg::fatal("failed to create surface!!!");
            return;
        }
    }
}

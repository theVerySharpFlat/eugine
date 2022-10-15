//
// Created by aiden on 10/8/22.
//

#include "VkFramebuffer.h"
#include "VkDevice.h"
#include "VKAPI.h"
#include "VkWindow.h"
#include <vulkan/vulkan_core.h>

namespace eg::rendering::VKWrapper {
    VkFramebuffer::VkFramebuffer(VKAPI& api, VkDevice& device, VkWindow& window) : m_api(api), m_device(device),
                                                                                   m_window(window) {}

    void VkFramebuffer::init(VkRenderPass& renderPass, VkSwapchainKHR swapchain, VkFormat imageFormat) {
        m_usage = DEFAULT; // this means that we are using this as the thing we present to the screen
        u32 swapchainImageCount;
        vkGetSwapchainImagesKHR(m_device.getDevice(), swapchain, &swapchainImageCount, nullptr);
        if(!swapchainImageCount) {
            error("failed to retrieve vulkan swapchain images!");
            m_initSuccess = false;
            return;
        }
        m_images.resize(swapchainImageCount);
        VkImage* tempSwapchainImages = (VkImage*) alloca(sizeof(VkImage) * m_images.size());
        vkGetSwapchainImagesKHR(m_device.getDevice(), swapchain, &swapchainImageCount, tempSwapchainImages);
        for(int i = 0; i < m_images.size(); i++) {
            auto& imageData = m_images[i];
            imageData = {.image = tempSwapchainImages[i]};
        }

        for(u32 i = 0; i < m_images.size(); i++) {
            VkImageViewCreateInfo imageViewCreateInfo{};
            imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            imageViewCreateInfo.image = m_images[i].image;
            imageViewCreateInfo.format = imageFormat;
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

            if(vkCreateImageView(m_device.getDevice(), &imageViewCreateInfo, nullptr, &m_images[i].imageView) != VK_SUCCESS) {
                error("failed to create vulkan swapchain image view {}", i);
                m_initSuccess = false;
                return;
            }
        }

        createFramebuffers(renderPass);
    }

    void VkFramebuffer::init(VkRenderPass& renderPass, Framebuffer::Usage usage, VmaAllocator allocator) {
        trace("renderpass in init: {}", (void*)renderPass.getRenderPass());
        m_allocator = allocator;
        m_usage = usage;
        m_images.resize(m_window.getSwapchainImageCount());
        VkImageCreateInfo imageCreateInfo{};
        imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
        imageCreateInfo.format = m_window.getSwapchainImageFormat();
        imageCreateInfo.extent.width = m_window.getSwapchainExtent().width;
        imageCreateInfo.extent.height = m_window.getSwapchainExtent().height;
        imageCreateInfo.extent.depth = 1;
        imageCreateInfo.mipLevels = 1;
        imageCreateInfo.arrayLayers = 1;
        imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageCreateInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

        VmaAllocationCreateInfo allocationCreateInfo{};
        allocationCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;
        allocationCreateInfo.flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
        allocationCreateInfo.priority = 1.0f;

        for (auto& imageData: m_images) {
            imageData = ImageData{};
            VkResult result = vmaCreateImage(m_allocator, &imageCreateInfo, &allocationCreateInfo, &imageData.image,
                                             &imageData.allocation, &imageData.allocationInfo);
            if (result != VK_SUCCESS) {
                error("failed to create image for offscreen rendering!!!");
                return;
            }

            VkImageViewCreateInfo imageViewCreateInfo{};
            imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            imageViewCreateInfo.image = imageData.image;
            imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            imageViewCreateInfo.format = m_window.getSwapchainImageFormat();
            imageViewCreateInfo.subresourceRange = {};
            imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
            imageViewCreateInfo.subresourceRange.levelCount = 1;
            imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
            imageViewCreateInfo.subresourceRange.layerCount = 1;

            if (vkCreateImageView(m_device.getDevice(), &imageViewCreateInfo, nullptr, &imageData.imageView) !=
                VK_SUCCESS) {
                error("failed to create image view for offscreen rendering!");
                return;
            }

            trace("image: {0:x}", (u64)imageData.image);
        }
        trace("renderpass before createFramebuffer: {}", (void*)renderPass.getRenderPass());

        createFramebuffers(renderPass);
    }

    VkSampler VkFramebuffer::sampler = VK_NULL_HANDLE;

    void VkFramebuffer::createGlobalSampler(VkDevice& device) {
        VkSamplerCreateInfo samplerCreateInfo{};
        samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
        samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
        samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        samplerCreateInfo.mipLodBias = 0.0f;
        samplerCreateInfo.maxAnisotropy = 1.0f;
        samplerCreateInfo.minLod = 0.0f;
        samplerCreateInfo.maxLod = 1.0f;
        samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;

        if (vkCreateSampler(device.getDevice(), &samplerCreateInfo, nullptr, &sampler) != VK_SUCCESS) {
            error("failed to create sampler for offscreen rendering!!!");
            return;
        }

    }

    void VkFramebuffer::destroyGlobalSampler(VkDevice& device) {
        vkDestroySampler(device.getDevice(), sampler, nullptr);
    }

    void VkFramebuffer::createFramebuffers(VkRenderPass& renderpass) {
        m_framebuffers.resize(m_images.size());

        for (int i = 0; i < m_images.size(); i++) {
            VkImageView attachments[] = {
                    m_images[i].imageView
            };

            VkFramebufferCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            createInfo.renderPass = renderpass.getRenderPass();
            createInfo.attachmentCount = 1;
            createInfo.pAttachments = attachments;
            createInfo.width = m_window.getSwapchainExtent().width;
            createInfo.height = m_window.getSwapchainExtent().height;
            createInfo.layers = 1;
            
            trace("renderpass in createFramebuffer: {}", (void*)renderpass.getRenderPass());

            if(vkCreateFramebuffer(m_device.getDevice(), &createInfo, nullptr, &m_framebuffers[i]) != VK_SUCCESS) {
                error("failed to create framebuffer {}!!!", i);
                m_initSuccess = false;
                return;
            }
        }
    }

    void VkFramebuffer::destruct() {
        m_device.waitIdle();
        trace("destroy fb");
        for(auto& frameBuffer : m_framebuffers) {
            if(frameBuffer != VK_NULL_HANDLE)
                vkDestroyFramebuffer(m_device.getDevice(), frameBuffer, nullptr);
            frameBuffer = VK_NULL_HANDLE;
        }

        trace("destroy image view");
        for(auto& image : m_images) {
            if(image.imageView != VK_NULL_HANDLE)
                vkDestroyImageView(m_device.getDevice(), image.imageView, nullptr);
            image.imageView = VK_NULL_HANDLE;
        }

        trace("destroy allocation");
        if(m_allocator != VK_NULL_HANDLE) {
            for (auto& image: m_images) {
                if (image.allocation != VK_NULL_HANDLE) {
                    vmaDestroyImage(m_allocator, image.image, image.allocation);
                    image.image = VK_NULL_HANDLE;
                    image.allocation = VK_NULL_HANDLE;
                    image.allocationInfo = {};
                }
            }
        }
    }
}

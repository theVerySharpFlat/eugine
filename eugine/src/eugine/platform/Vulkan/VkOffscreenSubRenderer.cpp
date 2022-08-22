//
// Created by aiden on 8/19/22.
//

#include "VkOffscreenSubRenderer.h"
#include "VkDevice.h"
#include "VkWindow.h"

#include "imgui/backends/imgui_impl_vulkan.h"
#include "imgui/imgui.h"

namespace eg::rendering::VKWrapper {
    extern void ImGui_ImplVulkan_CreatePipeline(VkDevice device, const VkAllocationCallbacks* allocator, VkPipelineCache pipelineCache, VkRenderPass renderPass, VkSampleCountFlagBits MSAASamples, VkPipeline* pipeline, uint32_t subpass);
    VkOffscreenSubRenderer::VkOffscreenSubRenderer(VkDevice& device, VkWindow& window, VmaAllocator& allocator)
            : m_device(device), m_window(window), m_allocator(allocator) {

    }

    VkOffscreenSubRenderer::~VkOffscreenSubRenderer() {

    }

    void VkOffscreenSubRenderer::init() {
        m_renderPass.init(true);

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

        if (vkCreateSampler(m_device.getDevice(), &samplerCreateInfo, nullptr, &m_sampler) != VK_SUCCESS) {
            error("failed to create sampler for offscreen rendering!!!");
            return;
        }


        m_frameBuffers.resize(m_window.getSwapchainImageCount());
        for (u32 i = 0; i < m_frameBuffers.size(); i++) {
            VkImageView attachment = m_images[i].imageView;

            VkFramebufferCreateInfo framebufferCreateInfo{};
            framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferCreateInfo.width = m_window.getSwapchainExtent().width;
            framebufferCreateInfo.height = m_window.getSwapchainExtent().height;
            framebufferCreateInfo.pAttachments = &attachment;
            framebufferCreateInfo.attachmentCount = 1;
            framebufferCreateInfo.layers = 1;
            framebufferCreateInfo.renderPass = m_renderPass.getRenderPass();

            if (vkCreateFramebuffer(m_device.getDevice(), &framebufferCreateInfo, nullptr, &m_frameBuffers[i]) !=
                VK_SUCCESS) {
                error("failed to create framebuffer {} for offscreen rendering", i);
                return;
            }
        }
    }


    void VkOffscreenSubRenderer::onWindowResize() {

    }

    void VkOffscreenSubRenderer::begin(VkCommandBuffer commandBuffer, u32 imageIndex) {
        VkClearValue clearValue{};
        clearValue.color = {{1.0f, 0.0f, 0.0f, 1.0f}};

        VkRenderPassBeginInfo renderPassBeginInfo{};
        renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassBeginInfo.renderPass = m_renderPass.getRenderPass();
        renderPassBeginInfo.framebuffer = m_frameBuffers[imageIndex];
        renderPassBeginInfo.renderArea.extent = m_window.getSwapchainExtent();
        renderPassBeginInfo.clearValueCount = 1;
        renderPassBeginInfo.pClearValues = &clearValue;

        vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
    }

    void VkOffscreenSubRenderer::drawOutputToWindow(const char* name, u32 imageIndex) {
        if (m_imguiDescriptorSets.empty()) {
            m_imguiDescriptorSets.resize(m_window.getSwapchainImageCount());
            for(auto& descriptor : m_imguiDescriptorSets) {
                descriptor = VK_NULL_HANDLE;
            }
        }

        if(m_imguiDescriptorSets[imageIndex] == VK_NULL_HANDLE)
            m_imguiDescriptorSets[imageIndex] = ImGui_ImplVulkan_AddTexture(m_sampler, m_images[imageIndex].imageView,
                                                                            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        ImGui::Begin(name);
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        ImGui::Image(m_imguiDescriptorSets[imageIndex], viewportPanelSize);
        ImGui::End();
    }

    void VkOffscreenSubRenderer::end(VkCommandBuffer commandBuffer, u32 imageIndex) {
        vkCmdEndRenderPass(commandBuffer);
    }

    void VkOffscreenSubRenderer::destruct() {
        vkDestroySampler(m_device.getDevice(), m_sampler, nullptr);
        for (auto& imageData: m_images) {
            vkDestroyImageView(m_device.getDevice(), imageData.imageView, nullptr);
            vmaDestroyImage(m_allocator, imageData.image, imageData.allocation);
        }
        vkDestroyRenderPass(m_device.getDevice(), m_renderPass.getRenderPass(), nullptr);
    }
}

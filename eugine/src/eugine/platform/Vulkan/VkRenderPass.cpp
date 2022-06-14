//
// Created by aiden on 6/14/22.
//

#include "VkRenderPass.h"

#include "VkWindow.h"
#include "VkDevice.h"

namespace eg::rendering::VKWrapper {
    VkRenderPass::VkRenderPass(VkDevice& device, VkWindow& window) : m_device(device), m_window(window) {}

    VkRenderPass::~VkRenderPass() {}

    void VkRenderPass::init() {
        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = m_window.m_swapchainImageFormat;
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentReference{};
        colorAttachmentReference.attachment = 0;
        colorAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpassDescription{};
        subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpassDescription.colorAttachmentCount = 1;
        subpassDescription.pColorAttachments = &colorAttachmentReference;

        VkRenderPassCreateInfo renderPassCreateInfo{};
        renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassCreateInfo.attachmentCount = 1;
        renderPassCreateInfo.pAttachments = &colorAttachment;
        renderPassCreateInfo.subpassCount = 1;
        renderPassCreateInfo.pSubpasses = &subpassDescription;

        if(vkCreateRenderPass(m_device.getDevice(), &renderPassCreateInfo, nullptr, &m_renderPass) != VK_SUCCESS) {
            fatal("failed to create renderpass!!!");
            return;
        }
    }

    void VkRenderPass::destruct() {
        vkDestroyRenderPass(m_device.getDevice(), m_renderPass, nullptr);
    }
}
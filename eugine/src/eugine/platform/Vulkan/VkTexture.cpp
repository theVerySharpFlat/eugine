//
// Created by aiden on 7/15/22.
//

#include "VkTexture.h"
#include "VkBufferUtil.h"
#include "VkCommandBufferUtil.h"
#include "VkDevice.h"

#include "stb_image.h"

#include "eugine/util/filesystem.h"

namespace eg::rendering::VKWrapper {
    VkTexture::VkTexture(VkDevice& device, VmaAllocator& allocator, VkCommandPool& commandPool) : m_device(device), m_allocator(allocator), m_commandPool(commandPool) {

    }

    VkTexture::~VkTexture() {
        destruct();
    }

    void
    VkTexture::transitionImageLayout(VkCommandBuffer commandBuffer, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) {
        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = oldLayout;
        barrier.newLayout = newLayout;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = image;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;

        VkPipelineStageFlagBits sourceStage, destinationStage;
        if(oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        } else if(oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        } else {
            error("invalid set of old and new layouts for image transition!!!");
            return;
        }

        vkCmdPipelineBarrier(
                commandBuffer,
                sourceStage, destinationStage,
                0,
                0, nullptr,
                0, nullptr,
                1, &barrier
                );
    }

    void
    VkTexture::copyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer buffer, VkImage image, u32 width, u32 height) {
        VkBufferImageCopy bufferImageCopy{};
        bufferImageCopy.bufferOffset = 0;
        bufferImageCopy.bufferImageHeight = 0;
        bufferImageCopy.bufferRowLength = 0;

        bufferImageCopy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        bufferImageCopy.imageSubresource.mipLevel = 0;
        bufferImageCopy.imageSubresource.baseArrayLayer = 0;
        bufferImageCopy.imageSubresource.layerCount = 1;

        bufferImageCopy.imageOffset = {0, 0 ,0};
        bufferImageCopy.imageExtent = {
                width,
                height,
                1
        };
        vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &bufferImageCopy);
    }

    void VkTexture::init(const char* path) {
        std::string imageFileContents = filesystem::getFileContents(path);

        if (imageFileContents.empty()) {
            error("failed to load file \"{}\"", path);
            return;
        }

        initFromData((u8*)imageFileContents.c_str(), imageFileContents.size(), path);

    }

    void VkTexture::initFromData(const u8* data, u32 size, const char* name) {

        i32 width, height, nrChannels;
        const unsigned char* img = stbi_load_from_memory(data, (int)size,
                                                         &width, &height, &nrChannels, STBI_rgb_alpha);
        m_width = width;
        m_height = height;
        m_numChannels = 4;
        u32 imgSize = m_width * m_height * m_numChannels * sizeof(u8);

        VkBuffer stagingBuffer;
        VmaAllocation stagingBufferAllocation;
        VmaAllocationInfo stagingBufferAllocationInfo;
        BufferUtil::createBuffer(m_allocator, VMA_MEMORY_USAGE_AUTO, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                                 BufferUtil::BUFFER_STAGING, imgSize, &stagingBuffer, &stagingBufferAllocation,
                                 &stagingBufferAllocationInfo);
        memcpy(stagingBufferAllocationInfo.pMappedData, img, imgSize);


        VkImageCreateInfo imageCreateInfo{};
        imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
        imageCreateInfo.extent.width = m_width;
        imageCreateInfo.extent.height = m_height;
        imageCreateInfo.extent.depth = 1;
        imageCreateInfo.mipLevels = 1;
        imageCreateInfo.arrayLayers = 1;
        imageCreateInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
        imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageCreateInfo.flags = 0;

        VmaAllocationCreateInfo allocationCreateInfo{};
        allocationCreateInfo.usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;

        VkResult imageCreateResult = vmaCreateImage(m_allocator, &imageCreateInfo, &allocationCreateInfo, &m_image, &m_imageAllocation, &m_imageAllocationInfo);
        if(imageCreateResult != VK_SUCCESS) {
            error("failed to create VkImage for texture \"{}\"", name);
            return;
        }

        VkCommandBuffer commandBuffer = CommandBufferUtil::beginSingleTimeCommands(m_device.getDevice(), m_commandPool);
        {
            transitionImageLayout(commandBuffer, m_image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
            copyBufferToImage(commandBuffer, stagingBuffer, m_image, m_width, m_height);
            transitionImageLayout(commandBuffer, m_image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        }
        VkFence fence = CommandBufferUtil::endSingleTimeCommands(m_device.getDevice(), m_device.getGraphicsQueue(), commandBuffer);
        vkWaitForFences(m_device.getDevice(), 1, &fence, VK_TRUE, UINT64_MAX);
        vkDestroyFence(m_device.getDevice(), fence, nullptr);
        vkFreeCommandBuffers(m_device.getDevice(), m_commandPool, 1, &commandBuffer);

        vmaDestroyBuffer(m_allocator, stagingBuffer, stagingBufferAllocation);

        VkImageViewCreateInfo imageViewCreateInfo{};
        imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewCreateInfo.image = m_image;
        imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        imageViewCreateInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
        imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
        imageViewCreateInfo.subresourceRange.layerCount = 1;
        imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
        imageViewCreateInfo.subresourceRange.levelCount = 1;
        imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

        if(vkCreateImageView(m_device.getDevice(), &imageViewCreateInfo, nullptr, &m_imageView) != VK_SUCCESS) {
            error("failed to create image view for \"{}\"", name);
            return;
        }

        VkSamplerCreateInfo samplerCreateInfo{};
        samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
        samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
        samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerCreateInfo.anisotropyEnable = VK_TRUE;
        samplerCreateInfo.maxAnisotropy = m_device.getPhysicalDeviceProperties().limits.maxSamplerAnisotropy;
        samplerCreateInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;
        samplerCreateInfo.compareEnable = VK_FALSE;
        samplerCreateInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerCreateInfo.mipLodBias = 0.0f;
        samplerCreateInfo.minLod = 0.0f;
        samplerCreateInfo.maxLod = 0.0f;

        if(vkCreateSampler(m_device.getDevice(), &samplerCreateInfo, nullptr, &m_sampler) != VK_SUCCESS) {
            error("failed to create sampler for texture \"{}\"");
            return;
        }
    }

    void VkTexture::destruct() {
        vkDestroySampler(m_device.getDevice(), m_sampler, nullptr);
        vkDestroyImageView(m_device.getDevice(), m_imageView, nullptr);
        vmaDestroyImage(m_allocator, m_image, m_imageAllocation);
    }

    bool VkTexture::operator==(const Texture& texture) const {
        auto vkTexture = (VkTexture&) texture; // pray conversion works. Seriously
        return vkTexture.m_image == m_image;
    }
}
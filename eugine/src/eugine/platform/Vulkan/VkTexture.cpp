//
// Created by aiden on 7/15/22.
//

#include "VkTexture.h"
#include "VkBufferUtil.h"

#include "stb_image.h"

#include "eugine/util/filesystem.h"

namespace eg::rendering::VKWrapper {
    VkTexture::VkTexture(VkDevice& device, VmaAllocator& allocator) : m_device(device), m_allocator(allocator) {

    }

    VkTexture::~VkTexture() {

    }

    void VkTexture::init(const char* path) {
        std::string imageFileContents = filesystem::getFileContents(path);

        if (imageFileContents.empty()) {
            error("failed to load file \"{}\"", path);
            return;
        }

        i32 width, height, nrChannels;
        const unsigned char* img = stbi_load_from_memory((uint8_t*) imageFileContents.c_str(), imageFileContents.size(),
                                                         &width, &height, &nrChannels, 4);
        m_width = width;
        m_height = height;
        m_numChannels = nrChannels;

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
            error("failed to create VkImage for texture \"{}\"", path);
            return;
        }

        vmaDestroyBuffer(m_allocator, stagingBuffer, stagingBufferAllocation);
    }

    void VkTexture::destruct() {

    }
}
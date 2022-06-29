//
// Created by aiden on 6/29/22.
//

#include "VkBufferUtil.h"
#include "VkDevice.h"

namespace eg::rendering::VKWrapper::BufferUtil {
    void createBuffer(VmaAllocator allocator, VmaMemoryUsage vmaMemoryUsage, VkBufferUsageFlags bufferUsage, u32 size,
                      VkBuffer* buffer, VmaAllocation* allocation) {
        VkBufferCreateInfo bufferCreateInfo{};
        bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferCreateInfo.size = size;
        bufferCreateInfo.usage = bufferUsage;

        VmaAllocationCreateInfo allocInfo{};
        allocInfo.usage = vmaMemoryUsage;

        if (vmaCreateBuffer(allocator, &bufferCreateInfo, &allocInfo, buffer, allocation, nullptr) != VK_SUCCESS) {
            error("failed to create vertex buffer!!!");
            return;
        }
    }

    void copyBuffer(VKWrapper::VkDevice& device, VkCommandPool commandPool, VkBuffer srcBuffer, VkBuffer dstBuffer,
                       VkDeviceSize size) {
        VkCommandBufferAllocateInfo commandBufferAllocateInfo{};
        commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        commandBufferAllocateInfo.commandBufferCount = 1;
        commandBufferAllocateInfo.commandPool = commandPool;

        VkCommandBuffer tempCommandBuffer;
        vkAllocateCommandBuffers(device.getDevice(), &commandBufferAllocateInfo, &tempCommandBuffer);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(tempCommandBuffer, &beginInfo);

        VkBufferCopy copyRegion{};
        copyRegion.size = size;
        copyRegion.srcOffset = 0;
        copyRegion.dstOffset = 0;
        vkCmdCopyBuffer(tempCommandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

        vkEndCommandBuffer(tempCommandBuffer);

        VkFence fence;
        VkFenceCreateInfo fenceCreateInfo{};
        fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        vkCreateFence(device.getDevice(), &fenceCreateInfo, nullptr, &fence);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &tempCommandBuffer;
        vkQueueSubmit(device.getGraphicsQueue(), 1, &submitInfo, fence);

        vkWaitForFences(device.getDevice(), 1, &fence, VK_TRUE, UINT64_MAX);
        vkDestroyFence(device.getDevice(), fence, nullptr);

        vkFreeCommandBuffers(device.getDevice(), commandPool, 1, &tempCommandBuffer);
    }
}
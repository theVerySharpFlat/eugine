//
// Created by aiden on 7/17/22.
//

#include "VkDescriptorSetAllocator.h"
#include "VkDevice.h"

namespace eg::rendering::VKWrapper {
    VkDescriptorSetAllocator::VkDescriptorSetAllocator(VkDevice& device) : m_device(device) {

    }

    VkDescriptorSetAllocator::~VkDescriptorSetAllocator() {

    }

    void VkDescriptorSetAllocator::init(DescriptorPoolAllocationHints allocationHints) {
        m_allocationHints = allocationHints;
    }

    void VkDescriptorSetAllocator::destruct() {
        for (auto& pool: m_freePools) {
            vkDestroyDescriptorPool(m_device.getDevice(), pool, nullptr);
        }

        for (auto& pool: m_usedPools) {
            vkDestroyDescriptorPool(m_device.getDevice(), pool, nullptr);
        }
    }

    VkDescriptorPool VkDescriptorSetAllocator::getFreePool() {
        if (!m_freePools.empty()) {
            VkDescriptorPool pool = m_freePools.back();
            m_freePools.pop_back();
            m_usedPools.push_back(pool);
            return pool;
        } else {
            std::array<VkDescriptorPoolSize, 2> poolSizes{};
            poolSizes[0] = VkDescriptorPoolSize{};
            poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            poolSizes[0].descriptorCount = (u32) (m_allocationHints.setsPerPool *
                                                  (double) m_allocationHints.avgUniformBuffersPerSet);
            poolSizes[1] = VkDescriptorPoolSize{};
            poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            poolSizes[1].descriptorCount = (u32) (m_allocationHints.setsPerPool *
                                                  (double) m_allocationHints.avgCombinedSamplersPerSet);
            VkDescriptorPoolCreateInfo descriptorPoolCreateInfo{};
            descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
            descriptorPoolCreateInfo.maxSets = m_allocationHints.setsPerPool;
            descriptorPoolCreateInfo.poolSizeCount = 2;
            descriptorPoolCreateInfo.pPoolSizes = poolSizes.data();

            VkDescriptorPool descriptorPool;
            if(vkCreateDescriptorPool(m_device.getDevice(), &descriptorPoolCreateInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
                error("failed to create descriptor pool!");
                return VK_NULL_HANDLE;
            }

            m_usedPools.push_back(descriptorPool);

            return descriptorPool;
        }
    }

    bool VkDescriptorSetAllocator::allocateDescriptorSet(VkDescriptorSet* descriptorSet,
                                                         VkDescriptorSetLayout descriptorSetLayout) {
        if(m_currentFreePool == VK_NULL_HANDLE) {
            m_currentFreePool = getFreePool();
        }

        VkDescriptorSetAllocateInfo descriptorSetAllocateInfo{};
        descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        descriptorSetAllocateInfo.pSetLayouts = &descriptorSetLayout;
        descriptorSetAllocateInfo.descriptorPool = m_currentFreePool;
        descriptorSetAllocateInfo.descriptorSetCount = 1;

        switch(vkAllocateDescriptorSets(m_device.getDevice(), &descriptorSetAllocateInfo, descriptorSet)) {
            case VK_SUCCESS:
                return true;
            case VK_ERROR_FRAGMENTED_POOL:
            case VK_ERROR_OUT_OF_POOL_MEMORY:
            {
                m_currentFreePool = getFreePool();
                descriptorSetAllocateInfo.descriptorPool = m_currentFreePool;

                if(vkAllocateDescriptorSets(m_device.getDevice(), &descriptorSetAllocateInfo, descriptorSet) != VK_SUCCESS) {
                    error("failed to allocate descriptor set on second try!!!");
                    return false;
                } else {
                    return true;
                }
            }
            break;
            default:
                error("something very wrong happened while allocating descriptor sets");
                return false;
        }
    }

    void VkDescriptorSetAllocator::resetAllocations() {
        while(!m_usedPools.empty()) {
            m_freePools.push_back(m_usedPools.back());
            m_usedPools.pop_back();
        }
        for (auto& pool: m_freePools) {
            vkResetDescriptorPool(m_device.getDevice(), pool, 0);
        }
    }
}
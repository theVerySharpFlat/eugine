//
// Created by aiden on 7/17/22.
//

#ifndef EUGINE_VKDESCRIPTORSETALLOCATOR_H
#define EUGINE_VKDESCRIPTORSETALLOCATOR_H

#include "volk.h"
#include <list>

namespace eg::rendering::VKWrapper {
    class VkDevice;

    struct DescriptorPoolAllocationHints {
        float avgUniformBuffersPerSet = 1.0f;
        float avgCombinedSamplersPerSet = 4.0f;
        u32 setsPerPool = 100;
    };

    class VkDescriptorSetAllocator {
    public:
        VkDescriptorSetAllocator(VkDevice& device);
        ~VkDescriptorSetAllocator();

        void init(DescriptorPoolAllocationHints allocationHints);
        void destruct();

        bool allocateDescriptorSet(VkDescriptorSet* descriptorSet, VkDescriptorSetLayout descriptorSetLayout);
        void resetAllocations();

    private:
        VkDevice& m_device;

        std::list<VkDescriptorPool> m_pools;
        std::list<VkDescriptorPool>::iterator m_poolIterator = m_pools.begin();

        VkDescriptorPool getFreePool();

        DescriptorPoolAllocationHints m_allocationHints{};
    };
}


#endif //EUGINE_VKDESCRIPTORSETALLOCATOR_H

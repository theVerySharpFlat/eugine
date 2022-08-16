//
// Created by aiden on 6/1/22.
//

#include "VkDevice.h"
#include "VKAPI.h"
#include "ValidationLayers.h"

#include <set>

namespace eg::rendering::VKWrapper {

    VkDevice::VkDevice(VKAPI& vkapi) : m_instance(vkapi.m_instance), m_surface(vkapi.m_vkWindow.getSurface()) {}

    VkDevice::~VkDevice() {}

    void VkDevice::initialize() {
        initializePhysicalDevice();
        initializeLogicalDevice();
    }

    void VkDevice::destruct() {
        if(m_device != VK_NULL_HANDLE)
            vkDestroyDevice(m_device, nullptr);
    }

    bool VkDevice::isDeviceSuitable(VkPhysicalDevice device) {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);

        // implement device ranking system when I get a discrete GPU
        info("starting vulkan");
        info("VULKAN DEVICE: {}", deviceProperties.deviceName);
        info("VULKAN API VERSION: {}.{}", VK_API_VERSION_MAJOR(deviceProperties.apiVersion),
             VK_API_VERSION_MINOR(deviceProperties.apiVersion));

        VkPhysicalDeviceFeatures physicalDeviceFeatures;
        vkGetPhysicalDeviceFeatures(device, &physicalDeviceFeatures);

        QueueFamilyIndices indices = findQueueFamilyIndices(device);
        return indices.isAcceptable() && deviceSupportsRequiredExtensions(device) && physicalDeviceFeatures.samplerAnisotropy;
    }

    bool VkDevice::deviceSupportsRequiredExtensions(VkPhysicalDevice device) {
        u32 extensionCount = 0;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
        if (extensionCount == 0) {
            eg::error("failed to retrieve device extensions!");
            return false;
        }
        auto properties = (VkExtensionProperties*) alloca(sizeof(VkExtensionProperties) * extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, properties);

        std::set<std::string> requiredExtensions(std::cbegin(deviceExtensions),
                                                 std::cbegin(deviceExtensions) + deviceExtensionsCount);

        u32 foundCount = 0;
        for (u32 i = 0; i < extensionCount; i++) {
            // trace("extensions: {}", properties[i].extensionName);
            const auto iterator = requiredExtensions.find(properties[i].extensionName);
            if (iterator != requiredExtensions.end()) {
                foundCount++;
            }
        }

        return foundCount == requiredExtensions.size();
    }

    void VkDevice::initializePhysicalDevice() {
        u32 deviceCount = 0;
        vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);
        if (deviceCount == 0) {
            eg::error("failed to find a Vulkan-capable GPU!!!");
            m_initSuccess = false;
            return;
        }
        auto* devices = (VkPhysicalDevice*) alloca(sizeof(VkPhysicalDevice) * deviceCount);
        vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices);

        for (u32 i = 0; i < deviceCount; i++) {
            if (isDeviceSuitable(devices[i])) {
                m_physicalDevice = devices[i];
            }
        }
        if (m_physicalDevice == VK_NULL_HANDLE) {
            eg::error("failed to find a suitable GPU!!!");
            m_initSuccess = false;
            return;
        }

        m_queueFamilyIndices = findQueueFamilyIndices(m_physicalDevice);
    }

    void VkDevice::initializeLogicalDevice() {
        QueueFamilyIndices queueIndices = findQueueFamilyIndices(m_physicalDevice);

        std::set<u32> uniqueQueues = {
                queueIndices.presentFamily.value(),
                queueIndices.graphicsFamily.value()
        };

        auto queueCreateInfos = (VkDeviceQueueCreateInfo*) alloca(
                sizeof(VkDeviceQueueCreateInfo) * uniqueQueues.size());
        float queuePriority = 1.0f;
        {
            u32 i = 0;
            for (u32 queue: uniqueQueues) {
                queueCreateInfos[i] = VkDeviceQueueCreateInfo{};
                queueCreateInfos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                queueCreateInfos[i].queueCount = 1;
                queueCreateInfos[i].queueFamilyIndex = queue;
                queueCreateInfos[i].pQueuePriorities = &queuePriority;
                i++;
            }
        }

        VkPhysicalDeviceFeatures deviceFeatures{};
        deviceFeatures.samplerAnisotropy = VK_TRUE;

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.pQueueCreateInfos = queueCreateInfos;
        createInfo.queueCreateInfoCount = uniqueQueues.size();
        createInfo.ppEnabledLayerNames = validationLayers;
        createInfo.enabledLayerCount = validationLayersCount;
        createInfo.enabledExtensionCount = deviceExtensionsCount;
        createInfo.ppEnabledExtensionNames = deviceExtensions;

        if (vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_device) != VK_SUCCESS) {
            eg::error("failed to create vulkan logical device!!!");
            m_initSuccess = false;
            return;
        }

        volkLoadDevice(m_device);

        vkGetDeviceQueue(m_device, queueIndices.graphicsFamily.value(), 0, &m_graphicsQueue);
        vkGetDeviceQueue(m_device, queueIndices.presentFamily.value(), 0, &m_presentQueue);
    }

    VkDevice::QueueFamilyIndices VkDevice::findQueueFamilyIndices(VkPhysicalDevice device) {
        QueueFamilyIndices indices{};
        u32 queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
        if (!queueFamilyCount) {
            eg::error("vulkan physical device has no queue families!");
            m_initSuccess = false;
            return QueueFamilyIndices{};
        }
        auto queueFamilyProperties = (VkQueueFamilyProperties*) alloca(
                sizeof(VkQueueFamilyProperties) * queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilyProperties);

        for (u32 i = 0; i < queueFamilyCount; i++) {
            VkQueueFamilyProperties& properties = queueFamilyProperties[i];

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_surface, &presentSupport);

            if (presentSupport) {
                indices.presentFamily = i;
            }

            if (properties.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphicsFamily = i;
            }
        }
        return indices;
    }

    VkPhysicalDeviceProperties VkDevice::getPhysicalDeviceProperties() {
        static VkPhysicalDeviceProperties physicalDeviceProperties{};
        static bool found = false;

        if(!found) {
            vkGetPhysicalDeviceProperties(m_physicalDevice, &physicalDeviceProperties);
            found = true;
        }

        return physicalDeviceProperties;
    }
}

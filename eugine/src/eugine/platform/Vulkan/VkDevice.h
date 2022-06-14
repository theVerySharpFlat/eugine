//
// Created by aiden on 6/1/22.
//

#ifndef EUGINE_VKDEVICE_H
#define EUGINE_VKDEVICE_H

#include <volk.h>


namespace eg::rendering::VKWrapper {
    class VKAPI;
    class VkDevice {
    public:
        VkDevice(VKAPI& vkapi);

        void initialize();
        void destruct();

        ~VkDevice();

        VkPhysicalDevice& getPhysicalDevice() { return m_physicalDevice; };
        ::VkDevice& getDevice() { return m_device; }

        struct QueueFamilyIndices {
            std::optional<u32> graphicsFamily;
            std::optional<u32> presentFamily;

            inline bool isAcceptable() const {
                return graphicsFamily.has_value() && presentFamily.has_value();
            }

        };
        QueueFamilyIndices getQueueFamilyIndices() { return m_queueFamilyIndices; }


    private:
        friend class VKAPI;
        friend class VkWindow;

        void initializePhysicalDevice();
        void initializeLogicalDevice();

        VkInstance& m_instance;
        VkSurfaceKHR& m_surface;

        VkPhysicalDevice m_physicalDevice;
        ::VkDevice m_device;

        VkQueue m_graphicsQueue;
        VkQueue m_presentQueue;

        const u32 deviceExtensionsCount = 1;
        const char* deviceExtensions[1] = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        QueueFamilyIndices findQueueFamilyIndices(VkPhysicalDevice device);
        QueueFamilyIndices m_queueFamilyIndices;

        bool isDeviceSuitable(VkPhysicalDevice device);
        bool deviceSupportsRequiredExtensions(VkPhysicalDevice device);
    };
}

#endif //EUGINE_VKDEVICE_H

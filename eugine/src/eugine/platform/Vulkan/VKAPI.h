//
// Created by aiden on 5/20/22.
//

#ifndef EUGINE_VKAPI_H
#define EUGINE_VKAPI_H

#include <eugine/rendering/GraphicsAPI.h>
#include <eugine/core/window.h>

#include "volk.h"

namespace eg::rendering::VKWrapper {
    class VKAPI : public ::eg::rendering::GraphicsAPI {
    public:
        VKAPI(Window &window);

        ~VKAPI();

        void setClearColor(glm::vec3 color) override {}

        void clear() override {}

        void swapBuffers() override {}

        i32 getMaxTexturesPerShader() const override;

    private:
        Window &m_window;

        VkInstance m_instance;
        VkPhysicalDevice m_physicalDevice;
        VkDevice m_device;

        VkQueue m_graphicsQueue;
        VkQueue m_presentQueue;

        VkSurfaceKHR m_surface;

#ifdef NDEBUG
        const bool enableValidationLayers = false;
        static const u32 validationLayersCount = 0;
        const char** validationLayers = nullptr;
        void confirmValidationLayerSupport() {}
        void setupDebugMessenger() {}
#else
        const bool enableValidationLayers = true;
        static const u32 validationLayersCount = 1;
        const char *validationLayers[validationLayersCount] = {
                "VK_LAYER_KHRONOS_validation"
        };

        void confirmValidationLayerSupport();

        void setupDebugMessenger();

        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);

        VkDebugUtilsMessengerEXT m_debugMessenger;

#endif

        std::vector<const char *> getRequiredInstanceExtensions();

        const u32 deviceExtensionsCount = 1;
        const char* deviceExtensions[1] = {
               VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };
        void initializePhysicalDevice();
        void initializeLogicalDevice();

        struct QueueFamilyIndices {
            std::optional<u32> graphicsFamily;
            std::optional<u32> presentFamily;

            inline bool isAcceptable() const {
                return graphicsFamily.has_value() && presentFamily.has_value();
            }

        };

        QueueFamilyIndices findQueueFamilyIndices(VkPhysicalDevice device);

        bool isDeviceSuitable(VkPhysicalDevice device);
        bool deviceSupportsRequiredExtensions(VkPhysicalDevice device);

        void createSurface();
    };
}

#endif //EUGINE_VKAPI_H

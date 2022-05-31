//
// Created by aiden on 5/20/22.
//

#include "VKAPI.h"
#include "GLFW/glfw3.h"

namespace eg::rendering::VKWrapper {

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT severity,
            VkDebugUtilsMessageTypeFlagsEXT type,
            const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
            void *pUserData
    ) {
        if (severity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
            ::eg::error("Vulkan: {}", pCallbackData->pMessage);
        else if (severity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
            ::eg::warn("Vulkan: {}", pCallbackData->pMessage);
        else if (severity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
            ::eg::info("Vulkan: {}", pCallbackData->pMessage);
        else
            ::eg::trace("Vulkan: {}", pCallbackData->pMessage);

        return VK_FALSE;
    }

    static VkResult createDebugUtilsMessengerEXTProxy(VkInstance
                                                      instance,
                                                      const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                                      const VkAllocationCallbacks *pAllocator,
                                                      VkDebugUtilsMessengerEXT *pDebugMessenger) {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance,
                                                                               "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr) {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        } else {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    static void destroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
                                              const VkAllocationCallbacks *pAllocator) {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance,
                                                                                "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr) {
            func(instance, debugMessenger, pAllocator);
        }
    }

    i32 VKAPI::getMaxTexturesPerShader() const {
        VkPhysicalDeviceProperties props;
        vkGetPhysicalDeviceProperties(m_physicalDevice, &props);
        return (i32) props.limits.maxPerStageDescriptorSamplers;
    }

    VKAPI::VKAPI(Window &window) : m_window(window), m_instance(VK_NULL_HANDLE), m_debugMessenger(VK_NULL_HANDLE),
                                   m_physicalDevice(VK_NULL_HANDLE) {

        EG_ASSERT(volkInitialize() == VK_SUCCESS, "failed to initialize volk!!!");

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Vulkan App";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "Eugine";
        appInfo.apiVersion = VK_API_VERSION_1_0;

        std::vector<const char *> extensions = getRequiredInstanceExtensions();

        confirmValidationLayerSupport();
        VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo{};
        if (enableValidationLayers) {
            populateDebugMessengerCreateInfo(debugUtilsMessengerCreateInfo);
        }

        VkInstanceCreateInfo instanceCreateInfo{};
        instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceCreateInfo.pApplicationInfo = &appInfo;
        instanceCreateInfo.enabledExtensionCount = extensions.size();
        instanceCreateInfo.ppEnabledExtensionNames = extensions.data();
        instanceCreateInfo.enabledLayerCount = validationLayersCount;
        instanceCreateInfo.ppEnabledLayerNames = validationLayers;
        instanceCreateInfo.pNext = enableValidationLayers ? &debugUtilsMessengerCreateInfo : nullptr;
        EG_ASSERT(vkCreateInstance(&instanceCreateInfo, nullptr, &m_instance) == VK_SUCCESS,
                  "Failed to create instance!");
        volkLoadInstance(m_instance);

        setupDebugMessenger();

        initializePhysicalDevice();
        initializeLogicalDevice();
    }

    bool VKAPI::isDeviceSuitable(VkPhysicalDevice device) {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);

        // implement device ranking system when I get a discrete GPU
        info("VULKAN DEVICE: {}", deviceProperties.deviceName);
        info("VULKAN API VERSION: {}.{}", VK_API_VERSION_MAJOR(deviceProperties.apiVersion), VK_API_VERSION_MINOR(deviceProperties.apiVersion));

        QueueFamilyIndices indices = findQueueFamilyIndices(device);
        return indices.isAcceptable();
    }

    void VKAPI::initializePhysicalDevice() {
        u32 deviceCount = 0;
        vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);
        if(deviceCount == 0) {
            eg::fatal("failed to find a Vulkan-capable GPU!!!");
            return;
        }
        auto* devices = (VkPhysicalDevice*)alloca(sizeof(VkPhysicalDevice) * deviceCount);
        vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices);

        for(u32 i = 0; i < deviceCount; i++) {
            if(isDeviceSuitable(devices[i])) {
                m_physicalDevice = devices[i];
            }
        }
        if(m_physicalDevice == VK_NULL_HANDLE) {
            eg::fatal("failed to find a suitable GPU!!!");
            return;
        }

    }

    void VKAPI::initializeLogicalDevice() {
        QueueFamilyIndices queueIndices = findQueueFamilyIndices(m_physicalDevice);

        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueIndices.graphicsFamily.value();
        queueCreateInfo.queueCount = 1;
        float queuePriority = 1.0f;
        queueCreateInfo.pQueuePriorities = &queuePriority;

        VkPhysicalDeviceFeatures deviceFeatures{};

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.pQueueCreateInfos = &queueCreateInfo;
        createInfo.queueCreateInfoCount = 1;
        createInfo.ppEnabledLayerNames = validationLayers;
        createInfo.enabledLayerCount = validationLayersCount;
        createInfo.enabledExtensionCount = 0;

        if(vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_device) != VK_SUCCESS) {
            eg::fatal("failed to create vulkan logical device!!!");
            return;
        }

        volkLoadDevice(m_device);

        vkGetDeviceQueue(m_device, queueIndices.graphicsFamily.value(), 0, &m_graphicsQueue);
    }

    VKAPI::QueueFamilyIndices VKAPI::findQueueFamilyIndices(VkPhysicalDevice device) {
        QueueFamilyIndices indices{};
        u32 queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
        if(!queueFamilyCount) {
            eg::fatal("vulkan physical device has no queue families!");
            return QueueFamilyIndices{};
        }
        auto queueFamilyProperties = (VkQueueFamilyProperties*)alloca(sizeof(VkQueueFamilyProperties) * queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilyProperties);

        for(u32 i = 0; i < queueFamilyCount; i++) {
            VkQueueFamilyProperties& properties = queueFamilyProperties[i];
            if(properties.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphicsFamily = i;
            }
        }
        return indices;
    }

    void VKAPI::confirmValidationLayerSupport() {
        u32 layerCount = 0;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        auto layers = (VkLayerProperties *) alloca(sizeof(VkLayerProperties) * layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, layers);
        for (int i = 0; i < validationLayersCount; i++) {
            bool layerFound = false;

            for (int j = 0; j < layerCount; j++) {

                if (strcmp(layers[j].layerName, validationLayers[i]) == 0) {
                    layerFound = true;
                    break;
                }
            }
            EG_ASSERT(layerFound, "layer \"{}\" not found", validationLayers[i]);
        }
    }

    void VKAPI::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo) {
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                     VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
                                     VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debugCallback;
        createInfo.pUserData = nullptr;
    }

    void VKAPI::setupDebugMessenger() {
        VkDebugUtilsMessengerCreateInfoEXT createInfo{};
        populateDebugMessengerCreateInfo(createInfo);
        EG_ASSERT(createDebugUtilsMessengerEXTProxy(m_instance, &createInfo, nullptr, &m_debugMessenger) == VK_SUCCESS,
                  "Failed to set up debug messenger!!!");
    }

    std::vector<const char *> VKAPI::getRequiredInstanceExtensions() {

        u32 glfwExtensionCount = 0;
        const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        if (enableValidationLayers)
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

        return extensions;
    }

    VKAPI::~VKAPI() {
        vkDestroyDevice(m_device, nullptr);
        if (enableValidationLayers) {
            destroyDebugUtilsMessengerEXT(m_instance, m_debugMessenger, nullptr);
        }
        vkDestroyInstance(m_instance, nullptr);
    }

}


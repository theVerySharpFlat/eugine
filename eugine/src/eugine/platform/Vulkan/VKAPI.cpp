//
// Created by aiden on 5/20/22.
//

#include <set>
#include "VKAPI.h"
#include "GLFW/glfw3.h"
#include "VkDevice.h"

namespace eg::rendering::VKWrapper {

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT severity,
            VkDebugUtilsMessageTypeFlagsEXT type,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData
    ) {
        if (severity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
            ::eg::error("Vulkan: {}", pCallbackData->pMessage);
        else if (severity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
            ::eg::warn("Vulkan: {}", pCallbackData->pMessage);
        else if (severity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
            ::eg::info("Vulkan: {}", pCallbackData->pMessage);
        else;
        // ::eg::trace("Vulkan: {}", pCallbackData->pMessage);

        return VK_FALSE;
    }

    static VkResult createDebugUtilsMessengerEXTProxy(VkInstance
                                                      instance,
                                                      const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                                      const VkAllocationCallbacks* pAllocator,
                                                      VkDebugUtilsMessengerEXT* pDebugMessenger) {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance,
                                                                               "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr) {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        } else {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    static void destroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
                                              const VkAllocationCallbacks* pAllocator) {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance,
                                                                                "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr) {
            func(instance, debugMessenger, pAllocator);
        }
    }

    i32 VKAPI::getMaxTexturesPerShader() const {
        VkPhysicalDeviceProperties props;
        vkGetPhysicalDeviceProperties(m_device.m_physicalDevice, &props);
        return (i32) props.limits.maxPerStageDescriptorSamplers;
    }

    VKAPI::VKAPI(Window& window) : m_window(window), m_instance(VK_NULL_HANDLE), m_debugMessenger(VK_NULL_HANDLE),
                                   m_device(*this), m_vkWindow(*this, m_device, m_renderPass, m_window),
                                   m_renderPass(m_device, m_vkWindow), m_shader(m_device, m_renderPass, m_vkWindow){

        EG_ASSERT(volkInitialize() == VK_SUCCESS, "failed to initialize volk!!!");

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Vulkan App";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "Eugine";
        appInfo.apiVersion = VK_API_VERSION_1_0;

        std::vector<const char*> extensions = getRequiredInstanceExtensions();

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

        m_vkWindow.initialize();
        m_device.initialize();

        m_vkWindow.createSwapchain();

        m_renderPass.init();

        const char* vertexShaderData ="#version 450\n"
                                      "\n"
                                      "layout(location = 0) out vec3 fragColor;\n"
                                      "\n"
                                      "vec2 positions[3] = vec2[](\n"
                                      "vec2(0.0, -0.5),\n"
                                      "vec2(0.5, 0.5),\n"
                                      "vec2(-0.5, 0.5)\n"
                                      ");\n"
                                      "\n"
                                      "vec3 colors[3] = vec3[](\n"
                                      "vec3(1.0, 0.0, 0.0),\n"
                                      "vec3(0.0, 1.0, 0.0),\n"
                                      "vec3(0.0, 0.0, 1.0)\n"
                                      ");\n"
                                      "\n"
                                      "void main() {\n"
                                      "gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);\n"
                                      "fragColor = colors[gl_VertexIndex];\n"
                                      "}";
        const char* fragmentShaderData = "#version 450\n"
                                           "\n"
                                           "layout(location = 0) in vec3 fragColor;\n"
                                           "layout(location = 0) out vec4 outColor;\n"
                                           "\n"
                                           "void main() {\n"
                                           "  outColor = vec4(fragColor, 1.0);\n"
                                           "}";


        m_shader.init({
                            {
                                "superBasic_vs",
                                vertexShaderData,
                                strlen(vertexShaderData)
                            },
                            {
                                "superBasic_fs",
                                fragmentShaderData,
                                strlen(fragmentShaderData)
                            }
        });

        m_vkWindow.createFrameBuffers();
    }


    void VKAPI::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
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

    std::vector<const char*> VKAPI::getRequiredInstanceExtensions() {

        u32 glfwExtensionCount = 0;
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        if (enableValidationLayers)
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

        return extensions;
    }


    VKAPI::~VKAPI() {
        m_vkWindow.destroyFrameBuffers();
        m_shader.destruct();
        m_renderPass.destruct();
        m_vkWindow.destroySwapchain();
        m_device.destruct();
        if (enableValidationLayers) {
            destroyDebugUtilsMessengerEXT(m_instance, m_debugMessenger, nullptr);
        }
        m_vkWindow.destruct();
        vkDestroyInstance(m_instance, nullptr);
    }

}


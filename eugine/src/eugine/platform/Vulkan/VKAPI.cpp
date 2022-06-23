//
// Created by aiden on 5/20/22.
//

#include <set>
#include "VKAPI.h"
#include "GLFW/glfw3.h"
#include "VkDevice.h"

static const char* vertexShaderData = "#version 450\n"
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
static const char* fragmentShaderData = "#version 450\n"
                                        "\n"
                                        "layout(location = 0) in vec3 fragColor;\n"
                                        "layout(location = 0) out vec4 outColor;\n"
                                        "\n"
                                        "void main() {\n"
                                        "  outColor = vec4(fragColor, 1.0);\n"
                                        "}";

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
                                   m_renderPass(m_device, m_vkWindow), m_shader(m_device, m_renderPass, m_vkWindow) {

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

        createCommandPool();
        allocateCommandBuffers();

        createSyncObjects();
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

    void VKAPI::createCommandPool() {
        VkCommandPoolCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        createInfo.queueFamilyIndex = m_device.getQueueFamilyIndices().graphicsFamily.value();

        if (vkCreateCommandPool(m_device.getDevice(), &createInfo, nullptr, &m_commandPool) != VK_SUCCESS) {
            error("failed to create command pool!");
            return;
        }
    }

    void VKAPI::allocateCommandBuffers() {
        VkCommandBufferAllocateInfo allocateInfo{};
        allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocateInfo.commandPool = m_commandPool;
        allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocateInfo.commandBufferCount = 1;

        if (vkAllocateCommandBuffers(m_device.getDevice(), &allocateInfo, &m_commandBuffer) != VK_SUCCESS) {
            error("failed to allocate command buffer");
            return;
        }
    }

    void VKAPI::createSyncObjects() {
        VkSemaphoreCreateInfo semaphoreCreateInfo{};
        semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        if (vkCreateSemaphore(m_device.getDevice(), &semaphoreCreateInfo, nullptr, &m_imageAvailableSemaphore) !=
            VK_SUCCESS) {
            error("failed to create imageAvailable semaphore");
            return;
        }

        if (vkCreateSemaphore(m_device.getDevice(), &semaphoreCreateInfo, nullptr, &m_renderFinishedSemaphore) !=
            VK_SUCCESS) {
            error("failed to create renderFinished semaphore");
            return;
        }

        VkFenceCreateInfo fenceCreateInfo{};
        fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        if (vkCreateFence(m_device.getDevice(), &fenceCreateInfo, nullptr, &m_inFlightFence) != VK_SUCCESS) {
            error("failed to create inFlightFence!!!");
            return;
        }
    }

    u32 VKAPI::acquireImage(bool& success) {
        while (true) {
            u32 imageIndex;
            VkResult result = vkAcquireNextImageKHR(m_device.getDevice(), m_vkWindow.m_swapchain, UINT64_MAX,
                                                    m_imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

            if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
                recreateSwapchain();
                continue;
            } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
                error("failed to acquire swapchain image!!!");
                success = false;
            } else {
                success = true;
            }

            return imageIndex;
        }
    }

    VKAPI::FrameData VKAPI::begin() {
        vkWaitForFences(m_device.getDevice(), 1, &m_inFlightFence, VK_TRUE, UINT64_MAX);
        vkResetFences(m_device.getDevice(), 1, &m_inFlightFence);

        bool imageAcquireSuccess;
        u32 imageIndex = acquireImage(imageAcquireSuccess);
        if (!imageAcquireSuccess) {
            return {UINT32_MAX};
        }


        vkResetCommandBuffer(m_commandBuffer, 0);
        beginCommandBufferRecording(imageIndex);

        return {imageIndex};
    }

    void VKAPI::beginCommandBufferRecording(u32 imageIndex) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0;
        beginInfo.pInheritanceInfo = 0;

        if (vkBeginCommandBuffer(m_commandBuffer, &beginInfo) != VK_SUCCESS) {
            error("failed to begin command buffer!");
        }

        VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
        // begin renderpass
        VkRenderPassBeginInfo renderPassBeginInfo{};
        renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassBeginInfo.renderPass = m_renderPass.m_renderPass;
        renderPassBeginInfo.framebuffer = m_vkWindow.m_framebuffers[imageIndex];
        renderPassBeginInfo.renderArea.offset = {0, 0};
        renderPassBeginInfo.renderArea.extent = m_vkWindow.m_swapchainExtent;
        renderPassBeginInfo.clearValueCount = 1;
        renderPassBeginInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(m_commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
    }

    void VKAPI::tempDraw() {
        vkCmdBindPipeline(m_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_shader.getPipeline());
        vkCmdDraw(m_commandBuffer, 3, 1, 0, 0);
    }

    void VKAPI::endCommandBufferRecording(u32 imageIndex) {
        vkCmdEndRenderPass(m_commandBuffer);

        if (vkEndCommandBuffer(m_commandBuffer) != VK_SUCCESS) {
            error("failed to end command buffer recording!!!");
            return;
        }
    }

    void VKAPI::end(VKAPI::FrameData frameData) {
        endCommandBufferRecording(frameData.imageIndex);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = {m_imageAvailableSemaphore};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &m_commandBuffer;

        VkSemaphore signalSemaphores[] = {m_renderFinishedSemaphore};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (vkQueueSubmit(m_device.m_graphicsQueue, 1, &submitInfo, m_inFlightFence) != VK_SUCCESS) {
            error("failed to submit queue!!!");
            return;
        }

        VkPresentInfoKHR presentInfoKhr{};
        presentInfoKhr.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfoKhr.waitSemaphoreCount = 1;
        presentInfoKhr.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapchains[] = {m_vkWindow.m_swapchain};
        presentInfoKhr.swapchainCount = 1;
        presentInfoKhr.pSwapchains = swapchains;
        presentInfoKhr.pImageIndices = &frameData.imageIndex;
        presentInfoKhr.pResults = nullptr;

        VkResult result = vkQueuePresentKHR(m_device.m_presentQueue, &presentInfoKhr);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
            recreateSwapchain();
            return;
        } else if (result != VK_SUCCESS) {
            error("failed to present swapchain image!");
        }

        int frameBufferWidth, frameBufferHeight;
        glfwGetFramebufferSize((GLFWwindow*) m_window.getNativeWindow(), &frameBufferWidth, &frameBufferHeight);

        if (m_vkWindow.m_swapchainExtent.width != frameBufferWidth ||
            m_vkWindow.m_swapchainExtent.height != frameBufferHeight) {
            trace("window resize");
            recreateSwapchain();
        }
    }

    void VKAPI::recreateSwapchain() {
        deviceWaitIdle();

        m_vkWindow.destroySwapchain();
        m_vkWindow.createSwapchain();
        m_renderPass.destruct();
        m_renderPass.init();
        m_shader.destruct();
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
        m_vkWindow.destroyFrameBuffers();
        m_vkWindow.createFrameBuffers();
    }

    VKAPI::~VKAPI() {
        vkDestroySemaphore(m_device.getDevice(), m_imageAvailableSemaphore, nullptr);
        vkDestroySemaphore(m_device.getDevice(), m_renderFinishedSemaphore, nullptr);
        vkDestroyFence(m_device.getDevice(), m_inFlightFence, nullptr);

        vkDestroyCommandPool(m_device.getDevice(), m_commandPool, nullptr);
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


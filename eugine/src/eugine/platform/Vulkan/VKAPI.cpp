//
// Created by aiden on 5/20/22.
//

#include <set>
#include "VKAPI.h"
#include "GLFW/glfw3.h"
#include "VkDevice.h"

static bool framebufferResized = false;
static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    framebufferResized = true;
}


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
        else
             ::eg::trace("Vulkan: {}", pCallbackData->pMessage);

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
                                   m_renderPass(m_device, m_vkWindow) {

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

        m_vkWindow.createFrameBuffers();

        createCommandPool();
        allocateCommandBuffers();

//        for(int i = 0; i < maxFramesInFlight; i++) {
//            auto& thing = m_frameObjects[i];
//            thing = FrameObjectsContainer{};
//            thing.m_commandBuffer = VK_NULL_HANDLE;
//            thing.m_inFlightFence = VK_NULL_HANDLE;
//            thing.m_imageAvailableSemaphore = VK_NULL_HANDLE;
//            thing.m_renderFinishedSemaphore = VK_NULL_HANDLE;
//        }
        createSyncObjects();

        glfwSetFramebufferSizeCallback((GLFWwindow*) m_window.getNativeWindow(), framebufferResizeCallback);
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

        for (auto& frameObjects: m_frameObjects) {
            if (vkAllocateCommandBuffers(m_device.getDevice(), &allocateInfo, &frameObjects.m_commandBuffer) !=
                VK_SUCCESS) {
                error("failed to allocate command buffer");
                return;
            }
            trace("command buffer create: {}", (void*)m_frameObjects[frameNumber].m_commandBuffer);
        }
    }

    void VKAPI::createSyncObjects() {
        VkSemaphoreCreateInfo semaphoreCreateInfo{};
        semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceCreateInfo{};
        fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (int i = 0; i < maxFramesInFlight; i++) {
            auto& frameObjects = m_frameObjects[i];
            trace("create objects");
            if (vkCreateSemaphore(m_device.getDevice(), &semaphoreCreateInfo, nullptr,
                                  &frameObjects.m_imageAvailableSemaphore) != VK_SUCCESS) {
                error("failed to create imageAvailable semaphore");
                return;
            }

            if (vkCreateSemaphore(m_device.getDevice(), &semaphoreCreateInfo, nullptr,
                                  &frameObjects.m_renderFinishedSemaphore) != VK_SUCCESS) {
                error("failed to create renderFinished semaphore");
                return;
            }


            if (vkCreateFence(m_device.getDevice(), &fenceCreateInfo, nullptr, &(frameObjects.m_inFlightFence)) != VK_SUCCESS) {
                error("failed to create inFlightFence!!!");
                return;
            }

            trace("fence on creation: {}", (void*) &frameObjects.m_inFlightFence);
        }
    }

    Ref<::eg::rendering::VKWrapper::VkShader> VKAPI::createShader(eg::rendering::Shader::ShaderProgramSource source) {
        auto temp = createRef<::eg::rendering::VKWrapper::VkShader>(m_device, m_renderPass, m_vkWindow);
        temp->init(source);

        return temp;
    }

    u32 VKAPI::acquireImage(bool& success) {
        while (true) {
            u32 imageIndex;
            VkResult result = vkAcquireNextImageKHR(m_device.getDevice(), m_vkWindow.m_swapchain, UINT64_MAX,
                                                    m_frameObjects[frameNumber].m_imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

            // trace("acquire image");
            if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
                glfwPollEvents();
                recreateSwapchain();
                continue;
            } else if (result != VK_SUCCESS) {
                error("failed to acquire swapchain image!!!");
                success = false;
            } else {
                success = true;
            }

            return imageIndex;
        }
    }

    VKAPI::FrameData VKAPI::begin() {
        vkWaitForFences(m_device.getDevice(), 1, &(m_frameObjects[frameNumber].m_inFlightFence), VK_TRUE, UINT64_MAX);

        bool imageAcquireSuccess = false;
        u32 imageIndex = acquireImage(imageAcquireSuccess);
        if (!imageAcquireSuccess) {
            return {UINT32_MAX};
        }
        // trace("acquired image");
        vkResetFences(m_device.getDevice(), 1, &(m_frameObjects[frameNumber].m_inFlightFence));

        // trace("command buffer reset: {}", (void*)m_frameObjects[frameNumber].m_commandBuffer);
        vkResetCommandBuffer(m_frameObjects[frameNumber].m_commandBuffer, 0);
        beginCommandBufferRecording(imageIndex);

        return {imageIndex};
    }

    void VKAPI::beginCommandBufferRecording(u32 imageIndex) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0;
        beginInfo.pInheritanceInfo = nullptr;

        VkCommandBuffer commandBuffer = m_frameObjects[frameNumber].m_commandBuffer;
        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
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

        vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
    }

    void VKAPI::tempDraw(Ref<VkShader> shader) {
        VkCommandBuffer commandBuffer = m_frameObjects[frameNumber].m_commandBuffer ;

        VkExtent2D swapchainExtent = m_vkWindow.getSwapchainExtent();
        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = swapchainExtent.width;
        viewport.height = swapchainExtent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

        VkRect2D scissorRect = {};
        scissorRect.extent = swapchainExtent;
        scissorRect.offset = {0, 0};
        vkCmdSetScissor(commandBuffer, 0, 1, &scissorRect);

        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, shader->getPipeline());
        vkCmdDraw(commandBuffer, 3, 1, 0, 0);
    }

    void VKAPI::endCommandBufferRecording(u32 imageIndex) {
        VkCommandBuffer commandBuffer = m_frameObjects[frameNumber].m_commandBuffer ;
        vkCmdEndRenderPass(commandBuffer);

        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            error("failed to end command buffer recording!!!");
            return;
        }
    }

    void VKAPI::end(VKAPI::FrameData frameData) {
        endCommandBufferRecording(frameData.imageIndex);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = {m_frameObjects[frameNumber].m_imageAvailableSemaphore};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        VkCommandBuffer commandBuffer = m_frameObjects[frameNumber].m_commandBuffer;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        VkSemaphore signalSemaphores[] = {m_frameObjects[frameNumber].m_renderFinishedSemaphore};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (vkQueueSubmit(m_device.m_graphicsQueue, 1, &submitInfo, m_frameObjects[frameNumber].m_inFlightFence) != VK_SUCCESS) {
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

        // trace("here");
        // trace("try end frame {}", frameNumber);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
            recreateSwapchain();
            framebufferResized = false;
            // return;
        } else if (result != VK_SUCCESS) {
            error("failed to present swapchain image!");
        }


        frameNumber++;
        frameNumber %= maxFramesInFlight;

        // trace("end. Next frame: {}", frameNumber);
    }

    void VKAPI::recreateSwapchain() {
        trace("recreate swapchain");
        glfwSetTime(0.0);
        deviceWaitIdle();

        m_vkWindow.updateSwapchainExtent();
        m_vkWindow.destroyFrameBuffers();
        m_renderPass.destruct();
        m_vkWindow.destroySwapchain();

        m_vkWindow.createSwapchain();
        m_renderPass.init();
        m_vkWindow.createFrameBuffers();
        trace("time to recreate: {}", glfwGetTime());
        // trace("here");
    }

    VKAPI::~VKAPI() {
        for(auto& frameObjects : m_frameObjects) {
            vkDestroySemaphore(m_device.getDevice(), frameObjects.m_imageAvailableSemaphore, nullptr);
            vkDestroySemaphore(m_device.getDevice(), frameObjects.m_renderFinishedSemaphore, nullptr);
            vkDestroyFence(m_device.getDevice(), frameObjects.m_inFlightFence, nullptr);
        }

        vkDestroyCommandPool(m_device.getDevice(), m_commandPool, nullptr);
        m_vkWindow.destroyFrameBuffers();
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


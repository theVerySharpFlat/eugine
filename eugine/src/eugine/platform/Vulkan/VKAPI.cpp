//
// Created by aiden on 5/20/22.
//

#include <set>
#include "VKAPI.h"
#include "GLFW/glfw3.h"
#include "VkDevice.h"
#include "VkShader.h"

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
        if (severity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
            ::eg::error("Vulkan: {}", pCallbackData->pMessage);
            // EG_DEBUG_BREAK;
        } else if (severity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
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

    VKAPI* VKAPI::singleton = nullptr;
    bool VKAPI::initSuccess = true;

    VKAPI::VKAPI(Window& window, bool renderOffScreen) : m_window(window), m_instance(VK_NULL_HANDLE),
                                   m_device(*this), m_vkWindow(*this, m_device, m_renderPass, m_window),
                                   m_renderPass(m_device, m_vkWindow), m_commandPool(VK_NULL_HANDLE),
                                   m_imguiSystem(*this), m_renderOffScreen(renderOffScreen) {

        EG_ASSERT(singleton == nullptr, "vulkan api already initialized!");
        singleton = this;

        if (volkInitialize() != VK_SUCCESS) {
            error("failed to initialize volk!!!");
            return;
        }

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Vulkan App";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "Eugine";
        appInfo.apiVersion = VK_API_VERSION_1_1;

        std::vector<const char*> extensions = getRequiredInstanceExtensions();

        confirmValidationLayerSupport();
        VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo{};
        if (enableValidationLayers) {
#ifdef EG_VK_VALIDATION
            populateDebugMessengerCreateInfo(debugUtilsMessengerCreateInfo);
#endif
        }

        VkInstanceCreateInfo instanceCreateInfo{};
        instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceCreateInfo.pApplicationInfo = &appInfo;
        instanceCreateInfo.enabledExtensionCount = extensions.size();
        instanceCreateInfo.ppEnabledExtensionNames = extensions.data();
        instanceCreateInfo.enabledLayerCount = validationLayersCount;
        instanceCreateInfo.ppEnabledLayerNames = validationLayers;
        instanceCreateInfo.pNext = enableValidationLayers ? &debugUtilsMessengerCreateInfo : nullptr;
        if (vkCreateInstance(&instanceCreateInfo, nullptr, &m_instance) != VK_SUCCESS) {
                error("Failed to create vulkan instance!");
                initSuccess = false;
                return;
        }
        volkLoadInstance(m_instance);

        if(!setupDebugMessenger()) {
            initSuccess = false;
            return;
        }

        m_vkWindow.initialize();
        if(!m_vkWindow.m_initSuccess) {
            initSuccess = false;
            return;
        }

        m_device.initialize();
        if(!m_device.m_initSuccess) {
            initSuccess = false;
            return;
        }

        m_vkWindow.createSwapchain();
        if(!m_vkWindow.m_initSuccess) {
            initSuccess = false;
            return;
        }

        m_renderPass.init(false);
        if(!m_renderPass.m_initSuccess) {
            initSuccess = false;
            return;
        }

        m_vkWindow.createFrameBuffers();
        if(!m_vkWindow.m_initSuccess) {
            initSuccess = false;
            return;
        }

        createCommandPool();
        if(m_commandPool == VK_NULL_HANDLE) {
            initSuccess = false;
            return;
        }
        allocateCommandBuffers();
        for(auto& objs : m_frameObjects) {
            if(objs.commandBuffer == VK_NULL_HANDLE) {
                initSuccess = false;
                return;
            }
        }

        if(!createSyncObjects()) {
            initSuccess = false;
            return;
        }

        glfwSetFramebufferSizeCallback((GLFWwindow*) m_window.getNativeWindow(), framebufferResizeCallback);

        createBufferAllocator();
        if(m_allocator == VK_NULL_HANDLE) {
            initSuccess = false;
            return;
        }

        for (auto& allocators: m_descriptorSetAllocators) {
            allocators.textureArrayAllocator.init({
                                                          0.0f, 128.0f, 20
                                                  });
            allocators.uniformBufferAllocator.init({
                                                           1.0f, 128.0f, 20
                                                   });
        }

        if(m_renderOffScreen)
            m_offscreenRenderer.init();

    }


#ifdef EG_VK_VALIDATION
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

    bool VKAPI::setupDebugMessenger() {
        VkDebugUtilsMessengerCreateInfoEXT createInfo{};
        populateDebugMessengerCreateInfo(createInfo);
        if(createDebugUtilsMessengerEXTProxy(m_instance, &createInfo, nullptr, &m_debugMessenger) != VK_SUCCESS) {
            error("failed to set up vulkan debug messenger");
            return false;
        }
        return true;
    }
#endif

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
            initSuccess = false;
            m_commandPool = VK_NULL_HANDLE;
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
            if (vkAllocateCommandBuffers(m_device.getDevice(), &allocateInfo, &frameObjects.commandBuffer) !=
                VK_SUCCESS) {
                error("failed to allocate command buffer");
                frameObjects.commandBuffer = VK_NULL_HANDLE;
                return;
            }
        }
    }

    bool VKAPI::createSyncObjects() {
        VkSemaphoreCreateInfo semaphoreCreateInfo{};
        semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceCreateInfo{};
        fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (auto& frameObjects: m_frameObjects) {
            if (vkCreateSemaphore(m_device.getDevice(), &semaphoreCreateInfo, nullptr,
                                  &frameObjects.imageAvailableSemaphore) != VK_SUCCESS) {
                error("failed to create imageAvailable semaphore");
                return false;
            }

            if (vkCreateSemaphore(m_device.getDevice(), &semaphoreCreateInfo, nullptr,
                                  &frameObjects.renderFinishedSemaphore) != VK_SUCCESS) {
                error("failed to create renderFinished semaphore");
                return false;
            }


            if (vkCreateFence(m_device.getDevice(), &fenceCreateInfo, nullptr, &(frameObjects.inFlightFence)) !=
                VK_SUCCESS) {
                error("failed to create inFlightFence!!!");
                return false;
            }

        }

        return true;
    }

    void VKAPI::createBufferAllocator() {
        VmaVulkanFunctions vulkanFunctions{};
        vulkanFunctions.vkGetInstanceProcAddr = vkGetInstanceProcAddr;
        vulkanFunctions.vkGetDeviceProcAddr = vkGetDeviceProcAddr;
        vulkanFunctions.vkGetPhysicalDeviceProperties = vkGetPhysicalDeviceProperties;
        vulkanFunctions.vkGetPhysicalDeviceMemoryProperties = vkGetPhysicalDeviceMemoryProperties;
        vulkanFunctions.vkAllocateMemory = vkAllocateMemory;
        vulkanFunctions.vkFreeMemory = vkFreeMemory;
        vulkanFunctions.vkMapMemory = vkMapMemory;
        vulkanFunctions.vkUnmapMemory = vkUnmapMemory;
        vulkanFunctions.vkFlushMappedMemoryRanges = vkFlushMappedMemoryRanges;
        vulkanFunctions.vkInvalidateMappedMemoryRanges = vkInvalidateMappedMemoryRanges;
        vulkanFunctions.vkBindBufferMemory = vkBindBufferMemory;
        vulkanFunctions.vkBindImageMemory = vkBindImageMemory;
        vulkanFunctions.vkGetBufferMemoryRequirements = vkGetBufferMemoryRequirements;
        vulkanFunctions.vkGetImageMemoryRequirements = vkGetImageMemoryRequirements;
        vulkanFunctions.vkCreateBuffer = vkCreateBuffer;
        vulkanFunctions.vkDestroyBuffer = vkDestroyBuffer;
        vulkanFunctions.vkCreateImage = vkCreateImage;
        vulkanFunctions.vkDestroyImage = vkDestroyImage;
        vulkanFunctions.vkCmdCopyBuffer = vkCmdCopyBuffer;
        vulkanFunctions.vkGetBufferMemoryRequirements2KHR = vkGetBufferMemoryRequirements2KHR;
        vulkanFunctions.vkGetImageMemoryRequirements2KHR = vkGetImageMemoryRequirements2KHR;
        vulkanFunctions.vkBindBufferMemory2KHR = vkBindBufferMemory2KHR;
        vulkanFunctions.vkBindImageMemory2KHR = vkBindImageMemory2KHR;
        vulkanFunctions.vkGetPhysicalDeviceMemoryProperties2KHR = vkGetPhysicalDeviceMemoryProperties2KHR;
        vulkanFunctions.vkGetDeviceBufferMemoryRequirements = vkGetDeviceBufferMemoryRequirements;
        vulkanFunctions.vkGetDeviceImageMemoryRequirements = vkGetDeviceImageMemoryRequirements;

        VmaAllocatorCreateInfo allocatorCreateInfo{};
        allocatorCreateInfo.physicalDevice = m_device.getPhysicalDevice();
        allocatorCreateInfo.device = m_device.getDevice();
        allocatorCreateInfo.instance = m_instance;
        allocatorCreateInfo.pVulkanFunctions = &vulkanFunctions;

        if (vmaCreateAllocator(&allocatorCreateInfo, &m_allocator) != VK_SUCCESS) {
            error("failed to create allocator!!!");
            m_allocator = VK_NULL_HANDLE;
            return;
        }
    }

    Ref<::eg::rendering::VKWrapper::VkShader>
    VKAPI::createShader(eg::rendering::Shader::ShaderProgramSource source, eg::rendering::VertexBufferLayout layout,
                        rendering::ShaderUniformLayout uniformLayout) {
        auto temp = createRef<::eg::rendering::VKWrapper::VkShader>(m_device, m_renderOffScreen ? m_offscreenRenderer.getRenderPass() : m_renderPass, m_vkWindow,
                                                                    m_descriptorSetAllocators.data(), maxFramesInFlight,
                                                                    frameNumber);
        temp->init(source, layout, uniformLayout);

        return temp;
    }

    Ref <VkVertexBuffer>
    VKAPI::createVertexBuffer(void* data, u32 size, VertexBufferLayout& layout,
                              rendering::VertexBuffer::UsageHints usageHint) {
        return eg::createRef<VkVertexBuffer>(m_device, m_commandPool, m_allocator, data, size, usageHint, layout);
    }

    Ref <VkIndexBuffer>
    VKAPI::createIndexBuffer(const u16* data, u32 count, rendering::VertexBuffer::UsageHints usageHint) {
        return eg::createRef<VkIndexBuffer>(m_device, m_commandPool, m_allocator, data, count, usageHint);
    }

    Ref <VkUniformBuffer> VKAPI::createUniformBuffer(void* data, u32 size, VertexBuffer::UsageHints usageHint) {
        return eg::createRef<VkUniformBuffer>(m_device, m_commandPool, m_allocator, data, size, usageHint);
    }

    Ref <VkTexture> VKAPI::createTexture(const char* path) {
        auto temp = createRef<VkTexture>(m_device, m_allocator, m_commandPool);
        temp->init(path);
        return temp;
    }

    Ref <VkTexture> VKAPI::createTextureFromData(const u8* data, u32 size, const char* name) {
        auto temp = createRef<VkTexture>(m_device, m_allocator, m_commandPool);
        temp->initFromData(data, size, name);
        return temp;
    }

    u32 VKAPI::acquireImage(bool& success) {
        while (true) {
            u32 imageIndex;
            VkResult result = vkAcquireNextImageKHR(m_device.getDevice(), m_vkWindow.getSwapchain(), UINT64_MAX,
                                                    m_frameObjects[frameNumber].imageAvailableSemaphore, VK_NULL_HANDLE,
                                                    &imageIndex);

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

    void VKAPI::begin() {
        // trace("waiting for frame {}", frameNumber);
        vkWaitForFences(m_device.getDevice(), 1, &(m_frameObjects[frameNumber].inFlightFence), VK_TRUE, UINT64_MAX);
        // trace("waiting for frame{} complete", frameNumber);
        // trace("resetting allocations for frame {}", frameNumber);
//        m_descriptorSetAllocators[frameNumber].uniformBufferAllocator.resetAllocations();
//        m_descriptorSetAllocators[frameNumber].textureArrayAllocator.resetAllocations();
        // trace("finished resetting allocations");

        bool imageAcquireSuccess = false;
        m_frameObjects[frameNumber].frameData.imageIndex = acquireImage(imageAcquireSuccess);
        if (!imageAcquireSuccess) {
            warn("vulkan: failed to aquire image!");
            return;
        }
        // trace("acquired image");
        vkResetFences(m_device.getDevice(), 1, &(m_frameObjects[frameNumber].inFlightFence));

        // trace("command buffer reset: {}", (void*)m_frameObjects[frameNumber].m_commandBuffer);
        vkResetCommandBuffer(m_frameObjects[frameNumber].commandBuffer, 0);
        beginCommandBufferRecording(m_frameObjects[frameNumber].frameData.imageIndex);
    }

    void VKAPI::beginCommandBufferRecording(u32 imageIndex) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0;
        beginInfo.pInheritanceInfo = nullptr;

        VkCommandBuffer commandBuffer = m_frameObjects[frameNumber].commandBuffer;
        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            error("failed to begin command buffer!");
        }

        if(!m_renderOffScreen) {
            VkClearValue clearColor = {{{m_clearColor.x, m_clearColor.y, m_clearColor.z, 1.0f}}};
            // begin renderpass
            VkRenderPassBeginInfo renderPassBeginInfo{};
            renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassBeginInfo.renderPass = m_renderPass.getRenderPass();
            renderPassBeginInfo.framebuffer = m_vkWindow.getFrameBuffer(imageIndex);
            renderPassBeginInfo.renderArea.offset = {0, 0};
            renderPassBeginInfo.renderArea.extent = m_vkWindow.getSwapchainExtent();
            renderPassBeginInfo.clearValueCount = 1;
            renderPassBeginInfo.pClearValues = &clearColor;

            vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
        } else {
            m_offscreenRenderer.begin(commandBuffer, imageIndex);
        }
    }

    void VKAPI::tempDraw(Ref <VkShader> shader) {
        VkCommandBuffer commandBuffer = m_frameObjects[frameNumber].commandBuffer;

        VkExtent2D swapchainExtent = m_vkWindow.getSwapchainExtent();
        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float) swapchainExtent.width;
        viewport.height = (float) swapchainExtent.height;
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

    void VKAPI::tempDraw(Ref <VkShader> shader, Ref <VkVertexBuffer> vertexBuffer) {

        VkCommandBuffer commandBuffer = m_frameObjects[frameNumber].commandBuffer;

        VkExtent2D swapchainExtent = m_vkWindow.getSwapchainExtent();
        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float) swapchainExtent.width;
        viewport.height = (float) swapchainExtent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

        VkRect2D scissorRect = {};
        scissorRect.extent = swapchainExtent;
        scissorRect.offset = {0, 0};
        vkCmdSetScissor(commandBuffer, 0, 1, &scissorRect);

        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, shader->getPipeline());

        VkBuffer vertexBuffers[] = {vertexBuffer->getBuffer()};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

        vkCmdDraw(commandBuffer, 3, 1, 0, 0);
    }

    void
    VKAPI::tempDrawIndexed(Ref <VkShader> shader, Ref <VkVertexBuffer> vertexBuffer, Ref <VkIndexBuffer> indexBuffer) {
        // trace("draw indexed on frame {}", frameNumber);

        VkCommandBuffer commandBuffer = m_frameObjects[frameNumber].commandBuffer;

        VkExtent2D swapchainExtent = m_vkWindow.getSwapchainExtent();
        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float) swapchainExtent.width;
        viewport.height = (float) swapchainExtent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

        VkRect2D scissorRect = {};
        scissorRect.extent = swapchainExtent;
        scissorRect.offset = {0, 0};
        vkCmdSetScissor(commandBuffer, 0, 1, &scissorRect);

        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, shader->getPipeline());

        if (shader->getPushConstantsSize() > 0) {
            vkCmdPushConstants(commandBuffer, shader->getPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0,
                               shader->getPushConstantsSize(), shader->getPushConstantsBuffer());
            trace("pconstants");
        }

        for (auto& descriptorInfo: shader->m_descriptorBindingNameToSetIndexMap) {
            if (descriptorInfo.second.needRebind) {
                vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, shader->getPipelineLayout(),
                                        descriptorInfo.second.setNum, 1,
                                        &descriptorInfo.second.descriptorSet, 0,
                                        nullptr);
                descriptorInfo.second.needRebind = false;
            }
        }

        VkBuffer vertexBuffers[] = {vertexBuffer->getBuffer()};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

        vkCmdBindIndexBuffer(commandBuffer, indexBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT16);

        //vkCmdDraw(commandBuffer, 3, 1, 0, 0);
        vkCmdDrawIndexed(commandBuffer, indexBuffer->getCount(), 1, 0, 0, 0);
    }

    void VKAPI::bindShader(Ref <VkShader> shader) {
        if (!shader) {
            m_currentBoundShader = nullptr;
            return;
        }
        VkCommandBuffer commandBuffer = m_frameObjects[frameNumber].commandBuffer;

        if (m_currentBoundShader) {
            m_currentBoundShader->resetDescriptorBindState();
        }

        VkExtent2D swapchainExtent = m_vkWindow.getSwapchainExtent();
        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float) swapchainExtent.width;
        viewport.height = (float) swapchainExtent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

        VkRect2D scissorRect = {};
        scissorRect.extent = swapchainExtent;
        scissorRect.offset = {0, 0};
        vkCmdSetScissor(commandBuffer, 0, 1, &scissorRect);

        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, shader->getPipeline());

        if (shader->getPushConstantsSize() > 0)
            vkCmdPushConstants(commandBuffer, shader->getPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0,
                               shader->getPushConstantsSize(), shader->getPushConstantsBuffer());

        m_currentBoundShader = shader;
    }

    void VKAPI::drawIndexed(Ref <VkVertexBuffer> vertexBuffer, Ref <VkIndexBuffer> indexBuffer) {
        VkCommandBuffer commandBuffer = m_frameObjects[frameNumber].commandBuffer;
        auto& shader = m_currentBoundShader;

        for (auto& descriptorInfo: shader->m_descriptorBindingNameToSetIndexMap) {
            if (descriptorInfo.second.needRebind) {
                vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, shader->getPipelineLayout(),
                                        descriptorInfo.second.setNum, 1,
                                        &descriptorInfo.second.descriptorSet, 0,
                                        nullptr);
                descriptorInfo.second.needRebind = false;
            }
        }

        VkBuffer vertexBuffers[] = {vertexBuffer->getBuffer()};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

        vkCmdBindIndexBuffer(commandBuffer, indexBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT16);

        vkCmdDrawIndexed(commandBuffer, indexBuffer->getCount(), 1, 0, 0, 0);
    }

    void VKAPI::endCommandBufferRecording(u32 imageIndex) {
        VkCommandBuffer commandBuffer = m_frameObjects[frameNumber].commandBuffer;
        vkCmdEndRenderPass(commandBuffer);

        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            error("failed to end command buffer recording!!!");
            return;
        }
    }

    void VKAPI::end() {
        endCommandBufferRecording(m_frameObjects[frameNumber].frameData.imageIndex);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = {m_frameObjects[frameNumber].imageAvailableSemaphore};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        VkCommandBuffer commandBuffer = m_frameObjects[frameNumber].commandBuffer;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        VkSemaphore signalSemaphores[] = {m_frameObjects[frameNumber].renderFinishedSemaphore};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (vkQueueSubmit(m_device.getGraphicsQueue(), 1, &submitInfo, m_frameObjects[frameNumber].inFlightFence) !=
            VK_SUCCESS) {
            error("failed to submit queue!!!");
            return;
        }

        VkPresentInfoKHR presentInfoKhr{};
        presentInfoKhr.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfoKhr.waitSemaphoreCount = 1;
        presentInfoKhr.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapchains[] = {m_vkWindow.getSwapchain()};
        presentInfoKhr.swapchainCount = 1;
        presentInfoKhr.pSwapchains = swapchains;
        presentInfoKhr.pImageIndices = &m_frameObjects[frameNumber].frameData.imageIndex;
        presentInfoKhr.pResults = nullptr;

        VkResult result = vkQueuePresentKHR(m_device.getPresentQueue(), &presentInfoKhr);

        // trace("here");
        // trace("try end frame {}", frameNumber);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
            recreateSwapchain();
            framebufferResized = false;
            // return;
        } else if (result != VK_SUCCESS) {
            error("failed to present swapchain image!");
        }

        // trace("end frame {}", frameNumber);
        frameNumber++;
        frameNumber %= maxFramesInFlight;

        // trace("end. Next frame: {}", frameNumber);
    }

    void VKAPI::recreateSwapchain() {
        // trace("recreate swapchain");
        // glfwSetTime(0.0);
        deviceWaitIdle();

        m_vkWindow.updateSwapchainExtent();
        m_vkWindow.destroyFrameBuffers();
        m_renderPass.destruct();
        m_vkWindow.destroySwapchain();

        m_vkWindow.createSwapchain();
        m_renderPass.init();
        m_vkWindow.createFrameBuffers();
        // trace("time to recreate: {}", glfwGetTime());
        // trace("here");
        m_imguiSystem.onSwapchainRecreation();
    }

    VKAPI::~VKAPI() {
        if (m_currentBoundShader != nullptr) {
            m_currentBoundShader = nullptr;
        }
        if(m_renderOffScreen)
            m_offscreenRenderer.destruct();
        if(m_allocator != VK_NULL_HANDLE)
            vmaDestroyAllocator(m_allocator);

        for (auto& allocators: m_descriptorSetAllocators) {
            allocators.textureArrayAllocator.destruct();
            allocators.uniformBufferAllocator.destruct();
        }

        // m_imguiSystem.shutdown();

        for (auto& frameObjects: m_frameObjects) {
            if(frameObjects.imageAvailableSemaphore != VK_NULL_HANDLE)
                vkDestroySemaphore(m_device.getDevice(), frameObjects.imageAvailableSemaphore, nullptr);
            if(frameObjects.renderFinishedSemaphore != VK_NULL_HANDLE)
                vkDestroySemaphore(m_device.getDevice(), frameObjects.renderFinishedSemaphore, nullptr);
            if(frameObjects.inFlightFence != VK_NULL_HANDLE)
                vkDestroyFence(m_device.getDevice(), frameObjects.inFlightFence, nullptr);
        }

        if(m_commandPool != VK_NULL_HANDLE)
            vkDestroyCommandPool(m_device.getDevice(), m_commandPool, nullptr);
        m_vkWindow.destroyFrameBuffers();
        m_renderPass.destruct();
        m_vkWindow.destroySwapchain();
        m_device.destruct();
        if (enableValidationLayers && m_debugMessenger != VK_NULL_HANDLE) {
            destroyDebugUtilsMessengerEXT(m_instance, m_debugMessenger, nullptr);
        }
        m_vkWindow.destruct();
        if(m_instance != VK_NULL_HANDLE)
            vkDestroyInstance(m_instance, nullptr);
    }

}


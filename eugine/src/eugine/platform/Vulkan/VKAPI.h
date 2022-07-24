//
// Created by aiden on 5/20/22.
//

#ifndef EUGINE_VKAPI_H
#define EUGINE_VKAPI_H

#include "ValidationLayers.h"

#include <eugine/rendering/GraphicsAPI.h>
#include <eugine/core/window.h>
#include <eugine/rendering/VertexBufferLayout.h>

#include "volk.h"
#include "VkDevice.h"
#include "VkWindow.h"
#include "VkRenderPass.h"
#include "VkVertexBuffer.h"
#include "VkIndexBuffer.h"
#include "VkTexture.h"
#include "VkDescriptorSetAllocator.h"
#include "VkUniformBuffer.h"
#include "VkImguiSystem.h"

#include "eugine/rendering/VertexBuffer.h"
#include "eugine/rendering/Shader.h"

namespace eg::rendering::VKWrapper {
    class VkDevice;

    class VkShader;

    class VKAPI : public ::eg::rendering::GraphicsAPI {
    public:
        VKAPI(Window& window);

        ~VKAPI();

        void setClearColor(glm::vec3 color) override { m_clearColor = color; }

        void clear() override {}

        void swapBuffers() override {}

        inline void deviceWaitIdle() { m_device.waitIdle(); }

        i32 getMaxTexturesPerShader() const override;

        struct FrameData { // to be passed between begin() and end() calls
            u32 imageIndex;
        };

        FrameData begin();

        void tempDraw(Ref<VkShader> shader);
        void tempDraw(Ref<VkShader> shader, Ref<VkVertexBuffer> vertexBuffer);
        void tempDrawIndexed(Ref<VkShader> shader, Ref<VkVertexBuffer> vertexBuffer, Ref<VkIndexBuffer> indexBuffer);

        void end(FrameData frameData);

        u32 acquireImage(bool& success);

        void recreateSwapchain();

        struct DescriptorSetAllocatorCombination {
            VkDescriptorSetAllocator uniformBufferAllocator;
            VkDescriptorSetAllocator textureArrayAllocator;
        };
        static inline const int maxFramesInFlight = 2;
        std::array<DescriptorSetAllocatorCombination, maxFramesInFlight> m_descriptorSetAllocators = {
            DescriptorSetAllocatorCombination {
                VkDescriptorSetAllocator(m_device),
                VkDescriptorSetAllocator(m_device)
            },
            DescriptorSetAllocatorCombination {
                VkDescriptorSetAllocator(m_device),
                VkDescriptorSetAllocator(m_device)
            }
        };

        Ref<::eg::rendering::VKWrapper::VkShader>
        createShader(eg::rendering::Shader::ShaderProgramSource source, eg::rendering::VertexBufferLayout layout, ShaderUniformLayout uniformLayout);

        Ref<VkVertexBuffer> createVertexBuffer(void* data, u32 size, rendering::VertexBuffer::UsageHints usageHint);
        Ref<VkIndexBuffer> createIndexBuffer(const u16* data, u32 count, rendering::VertexBuffer::UsageHints usageHint);
        Ref<VkUniformBuffer> createUniformBuffer(void* data, u32 size, VertexBuffer::UsageHints usageHint);

        Ref<VkTexture> createTexture(const char* path);

        u32 getFrameInFlight() const { return frameNumber; }

        VkImguiSystem& getImGuiSystem() { return m_imguiSystem; }

    private:
        friend class VkDevice;
        friend class VkWindow;
        friend class VkImguiSystem;

        Window& m_window;

        VkInstance m_instance;

        VkDevice m_device;

        VkWindow m_vkWindow;

        VkRenderPass m_renderPass;

        VkCommandPool m_commandPool;

        VkImguiSystem m_imguiSystem;

        VmaAllocator m_allocator;

        struct FrameObjectsContainer {
            VkCommandBuffer commandBuffer;
            VkSemaphore imageAvailableSemaphore;
            VkSemaphore renderFinishedSemaphore;
            VkFence inFlightFence;
        };
        FrameObjectsContainer m_frameObjects[maxFramesInFlight];
        u32 frameNumber = 0;

        glm::vec3 m_clearColor = {0.0f, 0.0f, 0.0f};

#ifdef NDEBUG
        void setupDebugMessenger() {}
#else

        void setupDebugMessenger();

        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

        VkDebugUtilsMessengerEXT m_debugMessenger;

#endif

        void createSyncObjects();

        void createCommandPool();

        void createBufferAllocator();

        void allocateCommandBuffers();

        void beginCommandBufferRecording(u32 imageIndex);

        void endCommandBufferRecording(u32 imageIndex);

        std::vector<const char*> getRequiredInstanceExtensions();

    };
}

#endif //EUGINE_VKAPI_H

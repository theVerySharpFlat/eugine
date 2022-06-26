//
// Created by aiden on 5/20/22.
//

#ifndef EUGINE_VKAPI_H
#define EUGINE_VKAPI_H

#include "ValidationLayers.h"

#include <eugine/rendering/GraphicsAPI.h>
#include <eugine/core/window.h>

#include "volk.h"
#include "VkDevice.h"
#include "VkWindow.h"
#include "VkRenderPass.h"
#include "VkShader.h"

namespace eg::rendering::VKWrapper {
    class VkDevice;

    class VKAPI : public ::eg::rendering::GraphicsAPI {
    public:
        VKAPI(Window &window);

        ~VKAPI();

        void setClearColor(glm::vec3 color) override {}

        void clear() override {}

        void swapBuffers() override {}

        inline void deviceWaitIdle() { m_device.waitIdle(); }

        i32 getMaxTexturesPerShader() const override;

        struct FrameData { // to be passed between begin() and end() calls
            u32 imageIndex;
        };

        FrameData begin();
        void tempDraw();
        void end(FrameData frameData);

        u32 acquireImage(bool& success);

        void recreateSwapchain();

        static const int maxFramesInFlight = 2;

    private:
        friend class VkDevice;
        friend class VkWindow;

        Window &m_window;

        VkInstance m_instance;

        VkDevice m_device;

        VkWindow m_vkWindow;

        VkRenderPass m_renderPass;

        VkShader m_shader;

        VkCommandPool  m_commandPool;

        struct FrameObjectsContainer {
            VkCommandBuffer m_commandBuffer;
            VkSemaphore m_imageAvailableSemaphore;
            VkSemaphore m_renderFinishedSemaphore;
            VkFence m_inFlightFence;
        };
        FrameObjectsContainer m_frameObjects[maxFramesInFlight];
        int frameNumber = 0;

#ifdef NDEBUG
        void setupDebugMessenger() {}
#else

        void setupDebugMessenger();

        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);

        VkDebugUtilsMessengerEXT m_debugMessenger;

#endif

        void createSyncObjects();

        void createCommandPool();
        void allocateCommandBuffers();

        void beginCommandBufferRecording(u32 imageIndex);
        void endCommandBufferRecording(u32 imageIndex);

        std::vector<const char *> getRequiredInstanceExtensions();

    };
}

#endif //EUGINE_VKAPI_H

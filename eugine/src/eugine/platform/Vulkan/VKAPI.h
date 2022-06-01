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

namespace eg::rendering::VKWrapper {
    class VkDevice;

    class VKAPI : public ::eg::rendering::GraphicsAPI {
    public:
        VKAPI(Window &window);

        ~VKAPI();

        void setClearColor(glm::vec3 color) override {}

        void clear() override {}

        void swapBuffers() override {}

        i32 getMaxTexturesPerShader() const override;

    private:
        friend class VkDevice;

        Window &m_window;

        VkInstance m_instance;

        VkDevice m_device;

        VkSurfaceKHR m_surface;

#ifdef NDEBUG
        void setupDebugMessenger() {}
#else

        void setupDebugMessenger();

        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);

        VkDebugUtilsMessengerEXT m_debugMessenger;

#endif

        std::vector<const char *> getRequiredInstanceExtensions();


        void createSurface();
    };
}

#endif //EUGINE_VKAPI_H

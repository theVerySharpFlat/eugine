//
// Created by aiden on 5/20/22.
//

#ifndef EUGINE_VKAPI_H
#define EUGINE_VKAPI_H

#include <eugine/rendering/GraphicsAPI.h>
#include <eugine/core/window.h>

#include "volk.h"

namespace eg::rendering::VKWrapper {
    class VKAPI : public ::eg::rendering::GraphicsAPI{
    public:
        VKAPI(Window& window);
        ~VKAPI();

        void setClearColor(glm::vec3 color) override {}
        void clear() override {}
        void swapBuffers() override {}
        i32 getMaxTexturesPerShader() const override { return 0; }

    private:
        Window& m_window;

        VkInstance m_instance;

#ifdef NDEBUG
        const bool enableValidationLayers = false;
        static const u32 validationLayersCount = 0;
        const char** validationLayers = nullptr;
        void confirmValidationLayerSupport() {}
        void setupDebugMessenger() {}
#else
        const bool enableValidationLayers = true;
        static const u32 validationLayersCount = 1;
        const char* validationLayers[validationLayersCount] = {
                "VK_LAYER_KHRONOS_validation"
        };
        void confirmValidationLayerSupport();

        void setupDebugMessenger();
        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        VkDebugUtilsMessengerEXT m_debugMessenger;
#endif

        std::vector<const char*> getRequiredInstanceExtensions();
    };
}

#endif //EUGINE_VKAPI_H

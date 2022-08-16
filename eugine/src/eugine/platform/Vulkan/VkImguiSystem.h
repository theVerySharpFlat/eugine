//
// Created by aiden on 7/23/22.
//

#ifndef EUGINE_VKIMGUISYSTEM_H
#define EUGINE_VKIMGUISYSTEM_H

#define IMGUI_IMPL_VULKAN_NO_PROTOTYPES
#include "volk.h"
#include "imgui/backends/imgui_impl_vulkan.h"

namespace eg::rendering::VKWrapper {
    class VKAPI;

    class VkImguiSystem {
    public:
        VkImguiSystem(VKAPI& api);
        ~VkImguiSystem();

        void init();
        void shutdown();

        void begin();
        void end();

        void onSwapchainRecreation();

    private:
        VKAPI& m_api;

        static PFN_vkVoidFunction imguiLoadFN(const char* name, void* userData);

        VkDescriptorPool m_pool;
    };
}


#endif //EUGINE_VKIMGUISYSTEM_H

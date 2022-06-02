//
// Created by aiden on 6/1/22.
//

#ifndef EUGINE_VKWINDOW_H
#define EUGINE_VKWINDOW_H

#include <volk.h>
#include <eugine/core/window.h>

namespace eg::rendering::VKWrapper {
    class VKAPI;
    class VkDevice;
    class VkWindow;

    class VkWindow {
    public:
        VkWindow(VKAPI& vkapi, VkDevice& device, Window& window);

        void initialize();
        void destruct();

        ~VkWindow();

        VkSurfaceKHR& getSurface() { return m_surface; }

    private:
        VkInstance& m_instance;
        // VkDevice& m_device;

        void* m_window;

        VkSurfaceKHR m_surface;

        void createSurface();
    };
}


#endif //EUGINE_VKWINDOW_H

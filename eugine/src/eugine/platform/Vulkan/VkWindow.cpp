//
// Created by aiden on 6/1/22.
//

#include "VkWindow.h"
#include "VKAPI.h"

#include <GLFW/glfw3.h>

namespace eg::rendering::VKWrapper {
    VkWindow::VkWindow(VKAPI& vkapi, VkDevice& device, Window& window) : m_instance(vkapi.m_instance),
                                                                         m_window(window.getNativeWindow()) {

    }

    void VkWindow::initialize() {
        createSurface();
    }

    void VkWindow::destruct() {
         vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
    }

    VkWindow::~VkWindow() {

    }

    void VkWindow::createSurface() {
        if (glfwCreateWindowSurface(m_instance, (GLFWwindow*) m_window, nullptr, &m_surface) != VK_SUCCESS) {
            eg::fatal("failed to create surface!!!");
            return;
        }
    }
}
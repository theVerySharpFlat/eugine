//
// Created by aiden on 7/28/22.
//

#include "RenderManager2D.h"
#include "GLFW/glfw3.h"

#include "eugine/core/window.h"
#include "eugine/core/application.h"

#define BIND_EVENT_FN(x) std::bind(&x, &Application::get(), std::placeholders::_1)

namespace eg::rendering {
    void RenderManager2D::init(Ref<Window>* window, Renderer2D::Settings renderSettings) {
        m_graphicsAPI = GraphicsAPI::create(**window);
        if(!GraphicsAPI::vulkanInitializationSuccessful()) {
            error("failed to initialize Vulkan!!!");
            info("trying to use OpenGL");
            WindowProps props = (*window)->getWindowProps();
            rendering::setPreferredGraphicsAPI(EG_API_OGL);
            window->reset();
            *window = Ref<Window>(Window::create(props));
            (*window)->setEventCallback(BIND_EVENT_FN(Application::onEvent));
            m_graphicsAPI = GraphicsAPI::create(**window);
        }
        m_renderer2D = new Renderer2D(*m_graphicsAPI, renderSettings);
        m_imguiLayer = new ImGuiLayer();
    }

    void RenderManager2D::shutdown() {
        m_graphicsAPI->deviceWaitIdle();
        delete m_renderer2D;
        delete m_graphicsAPI;
    }

    void RenderManager2D::imguiInit() {
        m_graphicsAPI->imguiInit();
    }

    void RenderManager2D::imguiShutdown() {
        m_graphicsAPI->deviceWaitIdle();
        m_graphicsAPI->imguiShutdown();
    }

    void RenderManager2D::imguiBegin() {
        m_graphicsAPI->imguiBegin();
        m_imguiLayer->begin();
    }

    void RenderManager2D::imguiEnd() {
        m_imguiLayer->end();
        m_graphicsAPI->imguiEnd();
        m_imguiLayer->updateWindows();
    }
}
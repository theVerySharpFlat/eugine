//
// Created by aiden on 7/28/22.
//

#include "RenderManager2D.h"

namespace eg::rendering {
    void RenderManager2D::init(Window* window, Renderer2D::Settings renderSettings) {
        m_graphicsAPI = GraphicsAPI::create(*window);
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
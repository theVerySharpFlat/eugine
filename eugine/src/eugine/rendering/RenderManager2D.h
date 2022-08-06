//
// Created by aiden on 7/28/22.
//

#ifndef EUGINE_RENDERMANAGER2D_H
#define EUGINE_RENDERMANAGER2D_H

#include "Renderer2D.h"
#include "eugine/core/window.h"
#include "eugine/imgui/imguiLayer.h"

namespace eg::rendering {
    class RenderManager2D {
    public:
        RenderManager2D() = default;
        ~RenderManager2D() = default;

        void begin() { m_graphicsAPI->begin(); }
        Renderer2D& renderer() { return *m_renderer2D; }
        void end() { m_graphicsAPI->end(); }

        void init(Window* window, Renderer2D::Settings renderSettings);
        void shutdown();

        ImGuiLayer* getImguiLayer() { return m_imguiLayer; }
        void imguiInit();
        void imguiShutdown();
        void imguiBegin();
        void imguiEnd();

    private:
        GraphicsAPI* m_graphicsAPI;
        Renderer2D* m_renderer2D;

        ImGuiLayer* m_imguiLayer;
    };
}


#endif //EUGINE_RENDERMANAGER2D_H

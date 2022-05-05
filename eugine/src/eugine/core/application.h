//
// Created by aiden on 8/2/21.
//

#ifndef EUGINE_APPLICATION_H
#define EUGINE_APPLICATION_H

#include <eugine/events/applicationEvent.h>
#include <eugine/imgui/imguiLayer.h>
#include "eugine/core/core.h"
#include "eugine/core/window.h"
#include "eugine/core/layerStack.h"

#include "eugine/platform/OpenGL/wrapper/Renderer.h"
#include "eugine/platform/OpenGL/wrapper/Texture.h"
#include "eugine/rendering/Camera2D.h"
#include "eugine/rendering/IndexBuffer.h"
#include "eugine/rendering/LowLevelRenderer.h"
#include "eugine/rendering/Texture.h"
#include "eugine/rendering/GraphicsAPI.h"

namespace eg {
    class EG_API Application {
    public:
        Application();

        virtual ~Application();

        void run();

        void onEvent(Event &e);

        virtual void init() {};

        void pushOverlay(Layer *layer);

        void pushLayer(Layer *layer);

        inline Window &getWindow() { return *m_window; }

        static Application &get();

    private:

        //windowing
        std::unique_ptr<Window> m_window;

        //layers
        ImGuiLayer *m_imGuiLayer;
        LayerStack m_layerStack;

        //event handling
        bool onWindowClose(WindowCloseEvent &e);

        bool m_running = true;

        //singleton class
        static Application *s_instance;


        Ref<rendering::Shader> m_shader = nullptr;


        float m_vertices[32] = {
            // positions          // colors           // texture coords
            600.0f,  600.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
            600.0f, 300.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
            200.0f, 300.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
            200.0f,  600.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
        };

        rendering::Camera2D m_camera = {
                glm::vec2(800.0f, 600.0f),
                glm::vec2(200.0f, 300.0f)
        };

        u32 m_indices[6] = {
                0, 1, 3,   // first triangle
                1, 2, 3    // second triangle
        };

        Ref<rendering::GraphicsAPI> m_renderAPI = nullptr;
        Ref<rendering::Texture> m_texture = nullptr;
        Ref<rendering::VertexBuffer> m_vbo = nullptr;
        Ref<rendering::VertexArray> m_vao = nullptr;
        Ref<rendering::IndexBuffer> m_ibo = nullptr;
        Ref<rendering::LowLevelRenderer> m_renderer = rendering::LowLevelRenderer::create();

        bool onKeyEvent(KeyPressedEvent& e);

    };

    Application *createApplication();
}

#endif //EUGINE_APPLICATION_H

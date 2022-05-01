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


        std::unique_ptr<GLWrapper::Shader> m_shader = nullptr;


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

        std::unique_ptr<GLWrapper::VertexBuffer> m_vbo = nullptr;

        std::unique_ptr<GLWrapper::VertexArray> m_vao = nullptr;

        std::unique_ptr<GLWrapper::IndexBuffer> m_ibo = nullptr;

        std::unique_ptr<GLWrapper::Texture> m_tex = nullptr;

        GLWrapper::Renderer m_renderer = {};

        bool onKeyEvent(KeyPressedEvent& e);

    };

    Application *createApplication();
}

#endif //EUGINE_APPLICATION_H

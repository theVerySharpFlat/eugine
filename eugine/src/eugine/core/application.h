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

namespace eg {
    class EG_API Application {
    public:
        Application();

        virtual ~Application();

        void run();

        void onEvent(Event& e);

        virtual void init() {  };

        void pushOverlay(Layer* layer);
        void pushLayer(Layer* layer);

        inline Window& getWindow() { return *m_window; }

        static Application& get();
    private:

        //windowing
        std::unique_ptr<Window>  m_window;

        //layers
        ImGuiLayer* m_imGuiLayer;
        LayerStack m_layerStack;

        //event handling
        bool onWindowClose(WindowCloseEvent& e);
        bool m_running = true;

        //singleton class
        static Application* s_instance;

        const char *vertexShaderSource = "#version 330 core\n"
                                         "layout (location = 0) in vec3 aPos;\n"
                                         "void main()\n"
                                         "{\n"
                                         "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                         "}\0";
        const char *fragmentShaderSource = "#version 330 core\n"
                                           "out vec4 FragColor;\n"
                                           "void main()\n"
                                           "{\n"
                                           "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                           "}\n\0";
        GLWrapper::ShaderUnitSource vertexUnitSource = {
                vertexShaderSource,
                sizeof(vertexShaderSource)
        };
        GLWrapper::ShaderUnitSource fragmentUnitSource = {
                fragmentShaderSource,
                sizeof(fragmentShaderSource)
        };
        GLWrapper::ShaderProgramSource m_shaderProgramSource = {
                vertexUnitSource,
                fragmentUnitSource
        };

        std::unique_ptr<GLWrapper::Shader> m_shader = nullptr;

        float m_vertices[9] = {
                -0.5f, -0.5f, 0.0f,
                0.5f, -0.5f, 0.0f,
                0.0f,  0.5f, 0.0f
        };
        std::unique_ptr<GLWrapper::VertexBuffer> m_vbo = nullptr;

        std::unique_ptr<GLWrapper::VertexArray> m_vao = nullptr;

        GLWrapper::Renderer m_renderer = {};

    };

    Application* createApplication();
}

#endif //EUGINE_APPLICATION_H

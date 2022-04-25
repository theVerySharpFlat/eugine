//
// Created by aiden on 8/2/21.
//


#include "application.h"
#include <eugine/events/applicationEvent.h>
#include <eugine/events/mouseEvent.h>
#include <eugine/core/input.h>

#include <glad/glad.h>

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

eg::Application* eg::Application::s_instance = nullptr;

eg::Application::Application() {

    //initialize log
    Logger::init();

    //singleton
    EG_CORE_ASSERT(!s_instance, "application already exists")
    s_instance = this;

    //windowing and events
    m_window = std::unique_ptr<Window>(Window::create());
    m_window ->setEventCallback(BIND_EVENT_FN(Application::onEvent));

    // OpenGL testing
    m_shader = std::make_unique<GLWrapper::Shader>(m_shaderProgramSource);

    m_vbo = std::make_unique<GLWrapper::VertexBuffer>(m_vertices, sizeof(m_vertices));

    m_vao = std::make_unique<GLWrapper::VertexArray>();

    m_ibo = std::make_unique<GLWrapper::IndexBuffer>(m_indices, sizeof(m_indices));

    GLWrapper::VertexBufferLayout vboLayout(1);
    vboLayout.setAttribute(0, {
       GL_FLOAT,
       3
    });
    m_vao->setBuffer(*m_vbo, vboLayout);

    //imgui
    m_imGuiLayer = new ImGuiLayer();
    pushOverlay(m_imGuiLayer);
}

eg::Application::~Application() {

}

void eg::Application::run() {
    while (m_running){
        glClearColor(1,1,0,1);
        glClear(GL_COLOR_BUFFER_BIT);
        for(Layer* layer : m_layerStack){
            layer -> onUpdate();
        }

        m_renderer.draw(*m_vao, *m_ibo, *m_shader);

        m_imGuiLayer->begin();
        for(Layer* layer : m_layerStack)
            layer -> onImGuiRender();
        m_imGuiLayer->end();
        m_window -> onUpdate();
    }
}

void eg::Application::onEvent(eg::Event &e) {
    EventDispatcher dispatcher(e);
    dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::onWindowClose));
    for(auto it = m_layerStack.end(); it != m_layerStack.begin(); ) {

        (*--it)->onEvent(e);
        if(e.handled){

            break;
        }
    }
}

void eg::Application::pushOverlay(eg::Layer *layer) {
    m_layerStack.pushOverlay(layer);
    layer -> onAttach();
}

void eg::Application::pushLayer(eg::Layer *layer) {
    m_layerStack.pushLayer(layer);
    layer -> onAttach();
}

bool eg::Application::onWindowClose(eg::WindowCloseEvent &e) {
    m_running = false;
    return true;
}

eg::Application& eg::Application::get() {
    return *s_instance;
}

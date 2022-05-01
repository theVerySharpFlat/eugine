//
// Created by aiden on 8/2/21.
//


#include "application.h"
#include "eugine/platform/OpenGL/wrapper/Texture.h"
#include "eugine/platform/OpenGL/wrapper/Shader.h"
#include "eugine/util/filesystem.h"
#include <eugine/events/applicationEvent.h>
#include <eugine/events/keyEvent.h>
#include <eugine/events/mouseEvent.h>
#include <eugine/core/input.h>
#include <eugine/core/keyCodes.h>

#include <glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

eg::Application* eg::Application::s_instance = nullptr;

eg::Application::Application() {

    //initialize log
    Logger::init();

    //singleton
    EG_CORE_ASSERT(!s_instance, "application already exists")
    s_instance = this;

    // go to project root
    filesystem::gotoProjectRoot();

    //windowing and events
    m_window = std::unique_ptr<Window>(Window::create());
    m_window ->setEventCallback(BIND_EVENT_FN(Application::onEvent));

    // OpenGL testing
    std::string vsData = filesystem::getFileContents("res/shaders/simple.vs");
    std::string fsData = filesystem::getFileContents("res/shaders/simple.fs");


    GLWrapper::ShaderProgramSource shaderSource = {
        {
            vsData.c_str(),
            vsData.size() + 1
        },
        {
            fsData.c_str(),
            fsData.size() + 1
        }
    };

    m_tex = std::make_unique<GLWrapper::Texture>("res/textures/brick.jpg");

    m_shader = std::make_unique<GLWrapper::Shader>(shaderSource);

    m_vbo = std::make_unique<GLWrapper::VertexBuffer>(m_vertices, sizeof(m_vertices));

    m_vao = std::make_unique<GLWrapper::VertexArray>();

    m_ibo = std::make_unique<GLWrapper::IndexBuffer>(m_indices, sizeof(m_indices));

    GLWrapper::VertexBufferLayout vboLayout(3);
    vboLayout.setAttribute(0, {
       GL_FLOAT,
       3
    });
    vboLayout.setAttribute(1, {
        GL_FLOAT,
        3
    });
    vboLayout.setAttribute(2, {
        GL_FLOAT,
        2
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

        const float moveSpeed = 2.0f;
        if(Input::isKeyPressed(EG_KEY_LEFT)) {
            m_camera.moveCamera({-moveSpeed, 0.0f});
        }
        if(Input::isKeyPressed(EG_KEY_RIGHT)) {
            m_camera.moveCamera({moveSpeed, 0.0f});
        }
        if(Input::isKeyPressed(EG_KEY_UP)) {
            m_camera.moveCamera({0.0f, moveSpeed});
        }
        if(Input::isKeyPressed(EG_KEY_DOWN)) {
            m_camera.moveCamera({0.0f, -moveSpeed});
        }

        m_tex->bind();
        m_shader->bind();
        m_shader -> setMat4("projxview", m_camera.getProjectionTimesView());
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
    dispatcher.dispatch<KeyPressedEvent>(BIND_EVENT_FN(Application::onKeyEvent));
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

bool eg::Application::onKeyEvent(eg::KeyPressedEvent &e) {
//    if(e.getKeyCode() == EG_KEY_RIGHT) {
//        m_camera.setPosition(m_camera.getPosition() + glm::vec2(0.0, 5.0));
//    } else if(e.getKeyCode() == EG_KEY_LEFT) {
//        m_camera.setPosition(m_camera.getPosition() + glm::vec2(0.0, -5.0));
//    }
    return true;
}

eg::Application& eg::Application::get() {
    return *s_instance;
}

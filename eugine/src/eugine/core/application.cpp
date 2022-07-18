//
// Created by aiden on 8/2/21.
//


#include "application.h"
#include "eugine/platform/OpenGL/wrapper/Texture.h"
#include "eugine/platform/OpenGL/wrapper/Shader.h"
#include "eugine/rendering/IndexBuffer.h"
#include "eugine/rendering/Texture.h"
#include "eugine/rendering/Types.h"
#include "eugine/rendering/VertexArray.h"
#include "eugine/rendering/VertexBuffer.h"
#include "eugine/rendering/VertexBufferLayout.h"
#include "eugine/util/filesystem.h"
#include <cmath>
#include <eugine/events/applicationEvent.h>
#include <eugine/events/keyEvent.h>
#include <eugine/events/mouseEvent.h>
#include <eugine/core/input.h>
#include <eugine/core/keyCodes.h>
#include <eugine/rendering/Shader.h>
#include <eugine/rendering/GraphicsAPI.h>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>
#include "eugine/platform/Vulkan/VKAPI.h"
#include <glm/gtx/string_cast.hpp>

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

eg::Application* eg::Application::s_instance = nullptr;

static const char* vertexShaderData = "#version 450\n"
                                      "\n"
                                      "layout(location = 0) in vec2 inPosition;\n"
                                      "layout(location = 1) in vec3 inColor;\n"
                                      "\n"
                                      "layout(location = 0) out vec3 fragColor;\n"
                                      ""
                                      "layout(push_constant) uniform constants {\n"
                                      "    mat4 u_projection;\n"
                                      "} pushConstants;\n"
                                      "\n"
                                      "void main() {\n"
                                      "vec4 pos = pushConstants.u_projection * vec4(inPosition, 0.0, 1.0);"
                                      "pos.y = -pos.y;\n"
                                      "gl_Position = pos;\n"
                                      "fragColor = inColor;\n"
                                      "}";
static const char* fragmentShaderData = "#version 450\n"
                                        "\n"
                                        "layout(location = 0) in vec3 fragColor;\n"
                                        "layout(location = 0) out vec4 outColor;\n"
                                        "\n"
                                        "void main() {\n"
                                        "  outColor = vec4(fragColor, 1.0);\n"
                                        "}";

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
    m_window->setEventCallback(BIND_EVENT_FN(Application::onEvent));

    m_renderAPI = rendering::GraphicsAPI::create(*m_window);
    info("Max Textures Per Shader: {}", m_renderAPI->getMaxTexturesPerShader());

    m_window->setVSync(true);

    // std::string vsData = filesystem::getFileContents("res/shaders/simple.vs");
    // std::string fsData = filesystem::getFileContents("res/shaders/simple.fs");

    // m_texture = rendering::Texture::create("res/textures/brick.jpg");

    const char* textureNames[56] = {
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_back.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_clubs_02.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_clubs_03.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_clubs_04.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_clubs_05.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_clubs_06.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_clubs_07.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_clubs_08.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_clubs_09.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_clubs_10.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_clubs_A.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_clubs_J.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_clubs_K.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_clubs_Q.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_diamonds_02.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_diamonds_03.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_diamonds_04.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_diamonds_05.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_diamonds_06.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_diamonds_07.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_diamonds_08.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_diamonds_09.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_diamonds_10.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_diamonds_A.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_diamonds_J.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_diamonds_K.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_diamonds_Q.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_empty.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_hearts_02.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_hearts_03.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_hearts_04.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_hearts_05.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_hearts_06.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_hearts_07.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_hearts_08.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_hearts_09.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_hearts_10.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_hearts_A.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_hearts_J.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_hearts_K.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_hearts_Q.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_joker_black.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_joker_red.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_spades_02.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_spades_03.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_spades_04.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_spades_05.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_spades_06.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_spades_07.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_spades_08.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_spades_09.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_spades_10.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_spades_A.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_spades_J.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_spades_K.png",
            "res/textures/playing-cards-pack/PNG/Cards (large)/card_spades_Q.png"
    };
    /* for(int i = 0; i < m_textures.size(); i++) {
        m_textures[i] = rendering::Texture::create(textureNames[i]);
    }

    m_renderer2 = rendering::Renderer2D::create({500, m_renderAPI->getMaxTexturesPerShader()});

    //imgui
    m_imGuiLayer = new ImGuiLayer();
    pushOverlay(m_imGuiLayer);*/
}

eg::Application::~Application() {}

void eg::Application::run() {
    auto vkGraphics = (eg::rendering::VKWrapper::VKAPI*) m_renderAPI.get();

    rendering::VertexBufferLayout vertexBufferLayout(2);
    vertexBufferLayout.setAttribute(0, { // pos
            rendering::SHDR_VEC2,
            1
    });
    vertexBufferLayout.setAttribute(1, { // color
            rendering::SHDR_VEC3,
            1
    });

    rendering::ShaderUniformLayout shaderUniformLayout = {
            {
                    {
                        "u_projection",
                        rendering::SHDR_MAT4
                    }
            },
            {
                    {
                        "u_matrices",
                        rendering::SHADER_BINDING_TYPE_UNIFORM_BUFFER,
                        1
                    }
            }
    };

    auto shader = vkGraphics->createShader({
                                                   {
                                                           "superBasic.vs", vertexShaderData,   strlen(vertexShaderData)
                                                   },
                                                   {
                                                           "superBasic.fs", fragmentShaderData, strlen(
                                                           fragmentShaderData)
                                                   }
                                           }, vertexBufferLayout, shaderUniformLayout);


    float vertices[] = {
            -150.0f, -150.0f, 1.0f, 0.0f, 0.0f, // bottom left
            150.0f, -150.0f, 1.0f, 0.0f, 0.0f, // bottom right
            150.0f, 150.0f, 1.0f, 0.0f, 0.0f, // top right
            -150.0f, 150.0f, 1.0f, 0.0f, 0.0f // top left
    };

    const u16 indices[] = {
            0, 1, 2, 2, 3, 0
    };

    auto vertexBuffer = vkGraphics->createVertexBuffer((void*) vertices, sizeof(vertices),
                                                       rendering::VertexBuffer::VB_USAGE_HINT_DYNAMIC);
    auto indexBuffer = vkGraphics->createIndexBuffer(indices, 6, rendering::VertexBuffer::VB_USAGE_HINT_STATIC);

    auto texture = vkGraphics->createTexture("res/textures/Vulkan/vulkan.png");

    while (m_running) {
        auto frameData = vkGraphics->begin();
        vkGraphics->tempDrawIndexed(shader, vertexBuffer, indexBuffer);
        vkGraphics->end(frameData);

        const float moveSpeed = 0.5f;
        if(Input::isKeyPressed(EG_KEY_LEFT)) {
            m_camera->moveCamera({-moveSpeed, 0.0f});
        }
        if(Input::isKeyPressed(EG_KEY_RIGHT)) {
            m_camera->moveCamera({moveSpeed, 0.0f});
        }
        if(Input::isKeyPressed(EG_KEY_UP)) {
            m_camera->moveCamera({0.0f, moveSpeed});
        }
        if(Input::isKeyPressed(EG_KEY_DOWN)) {
            m_camera->moveCamera({0.0f, -moveSpeed});
        }

        shader->setMat4("u_projection", m_camera->getProjectionTimesView());

//        static auto startTime = std::chrono::high_resolution_clock::now();
//
//        auto currentTime = std::chrono::high_resolution_clock::now();
//        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
//
//        float timeIntPart;
//        time = std::modf(time, &timeIntPart);
//
//        if((long)timeIntPart % 2) {
//            vertices[2] = vertices[7] = vertices[12] = vertices[17] = 1.0f;
//            vertices[3] = vertices[8] = vertices[13] = vertices[18] = 0.0f;
//        } else {
//            vertices[2] = vertices[7] = vertices[12] = vertices[17] = 0.0f;
//            vertices[3] = vertices[8] = vertices[13] = vertices[18] = 1.0f;
//        }
//
//
//        vertexBuffer->setData((void*) vertices, sizeof(vertices));
//        indexBuffer->setData(indices, 6);

        /* m_renderAPI->setClearColor({1.0, 0.0, 1.0});
        m_renderAPI->clear();
        for(Layer* layer : m_layerStack){
            layer -> onUpdate();
        }

        const float moveSpeed = 3.0f;
        if(Input::isKeyPressed(EG_KEY_LEFT)) {
            m_camera->moveCamera({-moveSpeed, 0.0f});
        }
        if(Input::isKeyPressed(EG_KEY_RIGHT)) {
            m_camera->moveCamera({moveSpeed, 0.0f});
        }
        if(Input::isKeyPressed(EG_KEY_UP)) {
            m_camera->moveCamera({0.0f, moveSpeed});
        }
        if(Input::isKeyPressed(EG_KEY_DOWN)) {
            m_camera->moveCamera({0.0f, -moveSpeed});
        }

        m_renderer2->begin(m_camera);

        auto now = std::chrono::high_resolution_clock::now();
        auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
        auto value = now_ms.time_since_epoch();
        double time = value.count();

        static double lastTime = time;

        glm::vec2 center = {
            30 * cos(time / 300),
            30 * sin(time / 300)
        };
        m_renderer2->submitQuad(center, {100, 100}, 0.0f, {1.0, 1.0, 1.0, 0.0}, m_texture);


        m_renderer2->submitQuad({-100, 100}, {100, 100}, 2 * M_PI * (time - lastTime) / 1000, {1.0f, 0.0f, 0.0f, 1.0f}, nullptr);
        m_renderer2->submitQuad({-100, 100}, {10, 10}, 0.0f, {0.0f, 0.0f, 1.0f, 1.0f}, nullptr);

        for(int i = 0; i < 10; i++) {
            for(int j = 0; j < 30; j++) {
                m_renderer2->submitQuad(
                        {j * 100, i * 100},
                        {100, 100},
                        2 * M_PI * (time - lastTime) / 2000,
                        {
                            (i + j) % 2 ? 0.0 : 1.0,
                            (i + j) % 2 ? 0.0 : 1.0,
                            (i + j) % 2 ? 1.0 : 1.0,
                            (i + j) % 2 ? 0.1 : 0.0
                        },
                            m_textures[(i * 30 + j) % 56]
                        );
            }
        }

        m_renderer2->submitQuad({200, 200}, {400, 400}, 0.0f, {0.0, 0.0, 1.0, 0.5}, nullptr);

        m_renderer2->end();
        // exit(0);

        m_imGuiLayer->begin();
        for(Layer* layer : m_layerStack)
            layer -> onImGuiRender();

        m_renderer2->imguiDbg();

        m_imGuiLayer->end();
        m_renderAPI->swapBuffers();*/
        m_window->onUpdate();
    }
    vkGraphics->deviceWaitIdle();
}

void eg::Application::onEvent(eg::Event& e) {
    EventDispatcher dispatcher(e);
    dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::onWindowClose));
    dispatcher.dispatch<KeyPressedEvent>(BIND_EVENT_FN(Application::onKeyEvent));
    for (auto it = m_layerStack.end(); it != m_layerStack.begin();) {

        (*--it)->onEvent(e);
        if (e.handled) {
            break;
        }
    }
}

void eg::Application::pushOverlay(eg::Layer* layer) {
    m_layerStack.pushOverlay(layer);
    layer->onAttach();
}

void eg::Application::pushLayer(eg::Layer* layer) {
    m_layerStack.pushLayer(layer);
    layer->onAttach();
}

bool eg::Application::onWindowClose(eg::WindowCloseEvent& e) {
    m_running = false;
    return true;
}

bool eg::Application::onKeyEvent(eg::KeyPressedEvent& e) {
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

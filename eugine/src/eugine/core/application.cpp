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
#include "eugine/platform/Vulkan/VkShader.h"
#include <glm/gtx/string_cast.hpp>
#include "eugine/rendering/Renderer2D.h"

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

eg::Application* eg::Application::s_instance = nullptr;

static const char* vertexShaderData = "#version 450\n"
                                      "\n"
                                      "layout(location = 0) in vec2 inPosition;\n"
                                      "layout(location = 1) in vec3 inColor;\n"
                                      "layout(location = 2) in vec2 inTexCoord;\n"
                                      "\n"
                                      "layout(location = 0) out vec3 fragColor;\n"
                                      "layout(location = 1) out vec2 fragTexCoord;\n"
                                      ""
                                      "layout(push_constant) uniform constants {\n"
                                      "    mat4 u_projection;\n"
                                      "} pushConstants;\n"
                                      ""
                                      "layout(binding = 0, set = 0) uniform UniformBufferObject {\n"
                                      "    mat4 model;"
                                      "} u_matrices;\n"
                                      "\n"
                                      "void main() {\n"
                                      "vec4 pos = pushConstants.u_projection * u_matrices.model * vec4(inPosition, 0.0, 1.0);"
                                      "pos.y = -pos.y;\n"
                                      "gl_Position = pos;\n"
                                      "fragColor = inColor;\n"
                                      "fragTexCoord = inTexCoord;"
                                      "}";
static const char* fragmentShaderData = "#version 450\n"
                                        "\n"
                                        "layout(location = 0) in vec3 fragColor;\n"
                                        "layout(location = 1) in vec2 texCoord;\n"
                                        "layout(location = 0) out vec4 outColor;\n"
                                        "\n"
                                        "layout(set = 1, binding = 0) uniform sampler2D texSampler;\n"
                                        "\n"
                                        "void main() {\n"
                                        "  outColor = texture(texSampler, texCoord);\n"
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

    //m_renderAPI = rendering::GraphicsAPI::create(*m_window);
    //info("Max Textures Per Shader: {}", m_renderAPI->getMaxTexturesPerShader());

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


    rendering::Renderer2D::Settings settings {
        1000,
        0,
        true
    };
    m_renderManager.init(m_window.get(), settings);
    pushOverlay(m_renderManager.getImguiLayer());
    m_renderManager.imguiInit();
    for(int i = 0; i < m_textures.size(); i++) {
        m_textures[i] = rendering::Texture::create(textureNames[i]);
    }
}

eg::Application::~Application() {
    for(auto& tex : m_textures)
        tex = nullptr;
    m_renderManager.imguiShutdown();
    m_renderManager.shutdown();
}

void eg::Application::run() {
    auto texture = rendering::Texture::create("res/textures/brick.jpg");

    while (m_running) {
        // trace("frame");
        static auto startTime = std::chrono::high_resolution_clock::now();
        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        // trace("frame begin");
        m_renderManager.begin();

            m_renderManager.renderer().begin(*m_camera);

            for(u32 i = 0; i < 100; i++) {
                for(u32 j = 0; j < 100; j++) {
                    m_renderManager.renderer().queueQuad({
                                                                 {100 * i, 100 * j},
                                                                 {100.0f, 100.0f},
                                                                 m_textures[(i * 100 + j) % 56],
                                                                 {1.0f, 1.0f},
                                                                 {1.0f, 1.0f, 0.0f, 0.5f},
                                                                 0.5f,
                                                                 time * glm::radians(180.0f)
                    });
                }
            }

        m_renderManager.renderer().end();

        m_renderManager.imguiBegin();
        m_renderManager.renderer().imguiDebug();
        for(Layer* layer : m_layerStack)
            layer -> onImGuiRender();
        m_renderManager.imguiEnd();

        m_renderManager.end();

        const float moveSpeed = 3.0f;
        if(Input::isKeyPressed(EG_KEY_LEFT)) {
            m_camera->moveCamera({-moveSpeed * time, 0.0f});
        }
        if(Input::isKeyPressed(EG_KEY_RIGHT)) {
            m_camera->moveCamera({moveSpeed * time, 0.0f});
        }
        if(Input::isKeyPressed(EG_KEY_UP)) {
            m_camera->moveCamera({0.0f, moveSpeed * time});
        }
        if(Input::isKeyPressed(EG_KEY_DOWN)) {
            m_camera->moveCamera({0.0f, -moveSpeed * time});
        }


        m_window->onUpdate();
    }

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
    return true;
}

eg::Application& eg::Application::get() {
    return *s_instance;
}

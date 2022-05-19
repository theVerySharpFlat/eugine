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
#include "eugine/rendering/Renderer2D.h"

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

        Ref<rendering::Camera2D> m_camera = createRef<rendering::Camera2D>(
                glm::vec2(1280.0f, 720.0f),
                glm::vec2(0.0f, 0.0f)
        );

        std::array<Ref<rendering::Texture>, 56> m_textures;

        Ref<rendering::GraphicsAPI> m_renderAPI = nullptr;
        Ref<rendering::Texture> m_texture = nullptr;

        Ref<rendering::Renderer2D> m_renderer2;

        bool onKeyEvent(KeyPressedEvent& e);

    };

    Application *createApplication();
}

#endif //EUGINE_APPLICATION_H

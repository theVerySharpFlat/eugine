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
        std::unique_ptr<Window>  m_window;
        ImGuiLayer* m_imGuiLayer;
        bool m_running = true;

        LayerStack m_layerStack;

        bool onWindowClose(WindowCloseEvent& e);

        static Application* s_instance;
    };

    Application* createApplication();
}

#endif //EUGINE_APPLICATION_H

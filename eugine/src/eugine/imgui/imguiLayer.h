//
// Created by aiden on 9/3/21.
//

#ifndef EUGINE_IMGUILAYER_H
#define EUGINE_IMGUILAYER_H

#include <eugine/layer.h>
#include <eugine/event/event.h>
#include <eugine/event/keyEvent.h>
#include <eugine/event/mouseEvent.h>
#include <eugine/event/applicationEvent.h>

namespace eg {

    class ImGuiLayer : public Layer{
    public:
        ImGuiLayer();

        ~ImGuiLayer() override;

        void onAttach() override;

        void onDetach() override;

        void onUpdate() override;

        void onEvent(Event &e) override;

    private:
        float m_time = 0;

        //TODO: window focus callback
        //TODO: cursor enter callback

        bool mouseButtonPressedCallback(MouseButtonPressedEvent& e);
        bool mouseButtonReleasedCallback(MouseButtonReleasedEvent& e);
        bool mouseMovedCallback(MouseMovedEvent& e);
        bool scrollCallback(MouseScrolledEvent& e);
        bool keyPressedCallback(KeyPressedEvent& e);
        bool keyReleasedCallback(KeyReleasedEvent& e);
        bool onKeyTypedEvent(KeyTypedEvent& e);
        bool windowResizeCallback(WindowResizeEvent& e);
    };

}

#endif //EUGINE_IMGUILAYER_H

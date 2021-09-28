//
// Created by aiden on 9/18/21.
//

#ifndef EUGINE_INPUT_H
#define EUGINE_INPUT_H

#include "core.h"

namespace eg {

    class EG_API Input {
    public:
        Input() { s_instance = this;}

        inline static bool isKeyPressed(int keyCode) { return s_instance -> isKeyPressedImpl(keyCode); }
        inline static bool isMouseButtonPressed(int mouseCode) { return s_instance ->isMouseButtonPressedImpl(mouseCode); }
        inline static std::pair<float, float> getMousePosition(){ return s_instance -> getMousePosImpl(); }
        inline static float getMouseX() { return s_instance -> getMouseXImpl(); }
        inline static float getMouseY() { return s_instance -> getMouseYImpl(); }
        //static bool isMouseButtonPressed(int mouseCode) { }

    protected:
        virtual bool isKeyPressedImpl(int keyCode) = 0;
        virtual bool isMouseButtonPressedImpl(int mouseCode) = 0;
        virtual std::pair<float, float> getMousePosImpl() = 0;
        virtual float getMouseXImpl() = 0;
        virtual float getMouseYImpl() = 0;
    private:
        static Input* s_instance;
    };

}

#endif //EUGINE_INPUT_H

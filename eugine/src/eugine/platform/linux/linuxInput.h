//
// Created by aiden on 9/18/21.
//

#ifndef EUGINE_LINUXINPUT_H
#define EUGINE_LINUXINPUT_H

#include "eugine/core/input.h"

namespace eg {
    class LinuxInput : public Input{

    protected:
    private:
        bool isKeyPressedImpl(int keyCode) override;

        bool isMouseButtonPressedImpl(int mouseCode) override;

        float getMouseXImpl() override;

        float getMouseYImpl() override;

        std::pair<float, float> getMousePosImpl() override;

    };
}


#endif //EUGINE_LINUXINPUT_H

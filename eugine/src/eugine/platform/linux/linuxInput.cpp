//
// Created by aiden on 9/18/21.
//

#include "linuxInput.h"

#include <GLFW/glfw3.h>
#include <eugine/core/application.h>

namespace eg{

    Input* Input::s_instance = new LinuxInput();

    bool LinuxInput::isKeyPressedImpl(int keyCode) {

        auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
        auto state = glfwGetKey(window, keyCode);

        return ((state == GLFW_PRESS) || (state == GLFW_REPEAT));
    }

    float LinuxInput::getMouseXImpl() {

        auto[x,y] = getMousePosImpl();
        return x;
    }

    float LinuxInput::getMouseYImpl() {
        auto[x,y] = getMousePosImpl();
        return y;
    }

    std::pair<float, float> LinuxInput::getMousePosImpl() {
        auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        return {(float)xpos, (float)ypos};
    }
}

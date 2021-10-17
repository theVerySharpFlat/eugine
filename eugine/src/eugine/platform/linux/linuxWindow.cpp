//
// Created by aiden on 8/17/21.
//

#include "linuxWindow.h"

#include <eugine/events/mouseEvent.h>
#include <eugine/events/applicationEvent.h>
#include <eugine/events/keyEvent.h>

#include <glad/glad.h>

namespace eg {

    static void GLFWErrorCallback(int error, const char* description){
        eg::error("GLFW erorr ({0}): {1}", error, description);
    }

    static bool s_GLFWInitialized = false;

    Window* Window::create(const WindowProps& props){
        return new LinuxWindow(props);
    }

    LinuxWindow::LinuxWindow(const WindowProps& props) {
        init(props);
    }

    void LinuxWindow::init(const WindowProps &props) {

        m_data.title  = props.title;
        m_data.width  = props.width;
        m_data.height = props.height;

        eg::info("Creating window \"{0}\" ({1}, {2})", m_data.title, m_data.width, m_data.height);

        if(!s_GLFWInitialized){
            int success = glfwInit();
            EG_CORE_ASSERT(success, "Could not initialize GLFW");
            glfwSetErrorCallback(GLFWErrorCallback);
            s_GLFWInitialized = true;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_window = glfwCreateWindow((int)m_data.width, (int)m_data.height, m_data.title.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(m_window);

        int status = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
        EG_CORE_ASSERT(status, "Failed to initialize Glad");

        glfwSetWindowUserPointer(m_window, &m_data);

        setVSync(true);

       //std::cout << glGetString(GL_VERSION) << std::endl;

        // set glfw callbacks
        glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            data.width = width;
            data.height = height;

            WindowResizeEvent event(width, height);
            data.eventCallback(event);
        });

        glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window){
           WindowData& data = *(WindowData*) glfwGetWindowUserPointer(window);
           WindowCloseEvent event = WindowCloseEvent();
           data.eventCallback(event);
        });

        glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            WindowData& data = *(WindowData*) glfwGetWindowUserPointer(window);

            switch (action) {
                case GLFW_PRESS:
                {
                    KeyPressedEvent event(key, 0);
                    data.eventCallback(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyPressedEvent event (key, 1);
                    data.eventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent event(key);
                    data.eventCallback(event);
                    break;
                }
            }
        });

        glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
            WindowData& data = *(WindowData*) glfwGetWindowUserPointer(window);

            switch (action) {

                case GLFW_PRESS:
                {
                    MouseButtonPressedEvent event(button);
                    data.eventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    MouseButtonReleasedEvent event(button);
                    data.eventCallback(event);
                    break;
                }
            }
        });

        glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset) {
            WindowData& data = *(WindowData*) glfwGetWindowUserPointer(window);

            MouseScrolledEvent((float)xOffset, float(yOffset));
        });

        glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos){
            WindowData& data = *(WindowData*) glfwGetWindowUserPointer(window);

            MouseMovedEvent event((float)xPos, (float)yPos);

            data.eventCallback(event);
        });

        glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int keycode) {
            WindowData& data = *(WindowData*) glfwGetWindowUserPointer(window);
            KeyTypedEvent event(keycode);
            data.eventCallback(event);
        });
    }

    void LinuxWindow::shutdown() {
        glfwDestroyWindow(m_window);
    }

    LinuxWindow::~LinuxWindow() {
        shutdown();
    }

    void LinuxWindow::onUpdate() {
        glfwPollEvents();
        glfwSwapBuffers(m_window);
    }

    unsigned int LinuxWindow::getWidth() const {
        return m_data.width;
    }

    unsigned int LinuxWindow::getHeight() const {
        return m_data.height;
    }

    void LinuxWindow::setEventCallback(const EventCallBackFn &callback) {
        m_data.eventCallback = callback;
    }

    void LinuxWindow::setVSync(bool enabled) {
        if (enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);

        m_data.vSync = enabled;
    }

    bool LinuxWindow::isVSync() {
        return m_data.vSync;
    }

    void *LinuxWindow::getNativeWindow() const {
        return m_window;
    }

}
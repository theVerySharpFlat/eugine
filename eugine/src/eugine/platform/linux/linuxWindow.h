//
// Created by aiden on 8/17/21.
//

#ifndef EUGINE_LINUXWINDOW_H
#define EUGINE_LINUXWINDOW_H

#include <GLFW/glfw3.h>
#include <eugine/core/core.h>
#include <eugine/core/window.h>
#include <eugine/events/event.h>

namespace eg {

    class EG_API LinuxWindow : public Window {
    public:

        LinuxWindow(const WindowProps& props);
        virtual ~LinuxWindow();

        void onUpdate() override;

        unsigned int getWidth() const override;

        unsigned int getHeight() const override;

        void setEventCallback(const EventCallBackFn &callback) override;

        void setVSync(bool enabled) override;

        bool isVSync() override;

        void *getNativeWindow() const override;

        WindowProps getWindowProps() const override;

    private:
        void init(const WindowProps& props);
        void shutdown();

        GLFWwindow* m_window;

        struct WindowData {
            std::string title;
            unsigned int width, height;
            bool vSync;

            EventCallBackFn eventCallback;
        };

        WindowData m_data;
    };

}

#endif //EUGINE_LINUXWINDOW_H

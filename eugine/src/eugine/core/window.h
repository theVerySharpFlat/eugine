//
// Created by aiden on 8/17/21.
//

#ifndef EUGINE_WINDOW_H
#define EUGINE_WINDOW_H

#include <eugine/core/core.h>
#include <eugine/events/event.h>

namespace eg{
    struct WindowProps{
        std::string title;
        unsigned int width;
        unsigned int height;

        WindowProps(const std::string& titleStr = "Eugine Engine", unsigned int inputWidth = 1280, unsigned int inputHeight = 720)
            : title(titleStr), width(inputWidth), height(inputHeight) {};
    };


    class EG_API Window {
    public:
        using EventCallBackFn = std::function<void(Event&)>;

        virtual ~Window() {};

        virtual void onUpdate() = 0;

        virtual unsigned int getWidth() const = 0;
        virtual unsigned int getHeight() const = 0;

        virtual void setEventCallback(const EventCallBackFn& callback) = 0;
        virtual void setVSync(bool enabled) = 0;
        virtual bool isVSync() = 0;

        static Window* create(const WindowProps& props = WindowProps());

        virtual void* getNativeWindow() const = 0;
    };
}

#endif //EUGINE_WINDOW_H

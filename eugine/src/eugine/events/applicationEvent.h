//
// Created by aiden on 8/16/21.
//

#ifndef EUGINE_APPLICATIONEVENT_H
#define EUGINE_APPLICATIONEVENT_H

#include <eugine/events/event.h>

namespace eg {
    class EG_API WindowResizeEvent : public Event {
    public:
        WindowResizeEvent(unsigned int width, unsigned int height) : m_width(width), m_height(height) {};

        inline unsigned int getWidth() { return m_width; }
        inline unsigned int getHeight() { return m_height; }

        std::string toString() const override {
            std::stringstream ss;
            ss << "WindowResizeEvent: " << m_width << ", " << m_height;
            return ss.str();
        }

        EVENT_CLASS_TYPE(WINDOW_RESIZE)

        EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APPLICATION)
    private:
        unsigned int m_width, m_height;
    };

    class EG_API WindowCloseEvent : public Event {
    public:
        WindowCloseEvent(){}

        std::string toString() const override{
            std::stringstream ss;
            ss << "Window Close Event";
            return ss.str();
        }

        EVENT_CLASS_TYPE(WINDOW_CLOSE)

        EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APPLICATION)
    };

    class EG_API AppTickEvent : public Event {
    public:
        AppTickEvent() {};

        EVENT_CLASS_TYPE(APP_TICK)
        EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APPLICATION)
    };

    class EG_API AppUpdateEvent : public Event {
        AppUpdateEvent() {};

        EVENT_CLASS_TYPE(APP_UPDATE)
        EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APPLICATION)
    };

    class EG_API AppRenderEvent : public Event {
    public:
        AppRenderEvent() {};

        EVENT_CLASS_TYPE(APP_RENDER)
        EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APPLICATION)
    };
}

#endif //EUGINE_APPLICATIONEVENT_H

//
// Created by aiden on 8/16/21.
//

#ifndef EUGINE_MOUSEEVENT_H
#define EUGINE_MOUSEEVENT_H

#include <eugine/events/event.h>

namespace eg {
    class EG_API MouseMovedEvent : public Event {
    public:
        MouseMovedEvent(float x, float y) : m_x(x), m_y(y) {};

        inline float getX() const {return m_x;}
        inline float getY() const {return m_y;}

        std::string toString() const override{
            std::stringstream ss;
            ss << "MouseMovedEvent: " << m_x << ", " << m_y;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MOUSE_MOVED)

        EVENT_CLASS_CATEGORY(EVENT_CATEGORY_INPUT | EVENT_CATEGORY_MOUSE)
    private:
        float m_x;
        float m_y;
    };

    class EG_API MouseScrolledEvent : public Event {
    public:
        MouseScrolledEvent(float xOffset, float yOffset) : m_xOffset(xOffset), m_yOffset(yOffset) {} ;

        inline float getXOffset() const { return m_xOffset; }
        inline float getYOffset() const { return m_yOffset; }

        std::string toString() const override {
            std::stringstream ss;
            ss << "MouseScrolledEvent: "<<m_yOffset<<", "<<m_yOffset;
        }

        EVENT_CLASS_TYPE(MOUSE_SCROLLED)

        EVENT_CLASS_CATEGORY(EVENT_CATEGORY_INPUT | EVENT_CATEGORY_MOUSE)

    private:
        float m_xOffset;
        float m_yOffset;
    };

    class EG_API MouseButtonEvent : public Event{

    public:
        inline int getMouseButton() const {return m_button;}

        EVENT_CLASS_CATEGORY(EVENT_CATEGORY_MOUSE | EVENT_CATEGORY_INPUT)
    protected:
        MouseButtonEvent(int button) : m_button(button) {};
        int m_button;
    };

    class EG_API MouseButtonPressedEvent : public MouseButtonEvent {
    public:
        MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {} ;

        std::string toString() const override {
            std::stringstream ss;
            ss << "MouseButtonPressedEvent: " << m_button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MOUSE_BUTTON_PRESSED)
    };

    class EG_API MouseButtonReleasedEvent : public MouseButtonEvent {
    public:
        MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {} ;

        std::string toString() const override {
            std::stringstream ss;
            ss << "MouseButtonReleasedEvent: " << m_button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MOUSE_BUTTON_RELEASED)
    };
}

#endif //EUGINE_MOUSEEVENT_H

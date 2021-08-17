//
// Created by aiden on 8/16/21.
//

#ifndef EUGINE_KEYEVENT_H
#define EUGINE_KEYEVENT_H

#include <eugine/event/event.h>

namespace eg {
    class EG_API KeyEvent : public Event {
    public:
        inline int getKeyCode() const {return m_keyCode;}

        EVENT_CLASS_CATEGORY(EVENT_CATEGORY_KEYBOARD | EVENT_CATEGORY_INPUT)
    protected:
        KeyEvent(int keycode) : m_keyCode(keycode) {};

        int m_keyCode;
    };

    class EG_API KeyPressedEvent : public KeyEvent {
    public:
        KeyPressedEvent(int keycode, int repeatCount) : KeyEvent(keycode), m_repeatCount(repeatCount) {};

        inline int getRepeatCount() const { return m_repeatCount; }

        std::string toString() const override {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << m_keyCode << " (" << m_repeatCount << " repeats)";
            return ss.str();
        }

        EVENT_CLASS_TYPE(KEY_PRESSED);

    private:
        int m_repeatCount;
    };

    class EG_API KeyReleasedEvent : public KeyEvent {

    public:
        KeyReleasedEvent(int keycode) : KeyEvent(keycode) {} ;

        std::string toString() const override {
            std::stringstream ss;
            ss << "KeyReleasedEvent: " << m_keyCode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KEY_RELEASED)
    };
}

#endif //EUGINE_KEYEVENT_H

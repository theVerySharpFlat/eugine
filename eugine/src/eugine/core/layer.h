//
// Created by aiden on 8/27/21.
//

#ifndef EUGINE_LAYER_H
#define EUGINE_LAYER_H

#include "eugine/core/core.h"
#include "eugine/events/event.h"


namespace eg {

    class EG_API Layer {
    public:
        Layer(const std::string& name = "layer");
        virtual ~Layer();

        virtual void onAttach() {};
        virtual void onDetach() {};
        virtual void onUpdate() {};
        virtual void onEvent(Event& e) {};

        inline const std::string getName() const { return m_debugName; }

    protected:
        std::string m_debugName;
    };
}

#endif //EUGINE_LAYER_H

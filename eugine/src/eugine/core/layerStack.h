//
// Created by aiden on 8/27/21.
//

#ifndef EUGINE_LAYERSTACK_H
#define EUGINE_LAYERSTACK_H

#include "eugine/core/layer.h"

namespace eg {

    class EG_API LayerStack {
    public:
        LayerStack();
        ~LayerStack();

        void pushLayer(Layer* layer);
        void pushOverlay(Layer* overlay);
        void popLayer(Layer* layer);
        void popOverlay(Layer* overlay);

        std::vector<Layer*>::iterator begin() { return m_layers.begin(); };
        std::vector<Layer*>::iterator end() { return m_layers.end(); };


    private:
        std::vector<Layer*> m_layers;
        std::vector<Layer*>::iterator  m_layerInsert;
    };

}

#endif //EUGINE_LAYERSTACK_H

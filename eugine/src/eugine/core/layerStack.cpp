//
// Created by aiden on 8/27/21.
//

#include "layerStack.h"

namespace eg {

    LayerStack::LayerStack() {

    }

    LayerStack::~LayerStack() {
        for(Layer* layer : m_layers) {
            delete layer;
        }
    };

    void LayerStack::pushLayer(Layer *layer) {
        //EG_CORE_TRACE("{0}", std::distance(m_layers.begin(), m_layerInsert));
        m_layers.emplace(m_layers.begin() + m_layerInsertIndex, layer);
        m_layerInsertIndex++;
    }

    void LayerStack::pushOverlay(Layer *overlay) {
        m_layers.emplace_back(overlay);
    }

    void LayerStack::popLayer(Layer *layer) {
        auto it = std::find(m_layers.begin(), m_layers.end(), layer);

        if(it != m_layers.end()){
            m_layers.erase(it);
            m_layerInsertIndex--;
        }
    }

    void LayerStack::popOverlay(Layer *overlay) {
        auto it = std::find(m_layers.begin(), m_layers.end(), overlay);
        if(it != m_layers.end()){
            m_layers.erase(it);
        }
    }
}
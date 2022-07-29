//
// Created by aiden on 7/28/22.
//

#include "RenderManager2D.h"

namespace eg::rendering {
    void RenderManager2D::init(Window* window, Renderer2D::Settings renderSettings) {
        graphicsAPI = GraphicsAPI::create(*window);
        renderer2D = new Renderer2D(*graphicsAPI, renderSettings);
    }

    void RenderManager2D::shutdown() {
        graphicsAPI->deviceWaitIdle();
        delete renderer2D;
        delete graphicsAPI;
    }
}
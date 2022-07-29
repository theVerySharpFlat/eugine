//
// Created by aiden on 7/28/22.
//

#ifndef EUGINE_RENDERMANAGER2D_H
#define EUGINE_RENDERMANAGER2D_H

#include "Renderer2D.h"

#include "eugine/core/window.h"

namespace eg::rendering {
    class RenderManager2D {
    public:
        RenderManager2D() = default;
        ~RenderManager2D() = default;

        void begin() { graphicsAPI->begin(); }
        Renderer2D& renderer() { return *renderer2D; }
        void end() { graphicsAPI->end(); }

        void init(Window* window, Renderer2D::Settings renderSettings);
        void shutdown();

    private:
        GraphicsAPI* graphicsAPI;
        Renderer2D* renderer2D;
    };
}


#endif //EUGINE_RENDERMANAGER2D_H

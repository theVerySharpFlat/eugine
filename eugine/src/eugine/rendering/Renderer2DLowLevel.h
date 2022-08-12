//
// Created by aiden on 7/24/22.
//

#ifndef EUGINE_RENDERER2DLOWLEVEL_H
#define EUGINE_RENDERER2DLOWLEVEL_H

#include "Renderer2D.h"
#include "Camera2D.h"
#include "Shader.h"

#include "GraphicsAPI.h"

namespace eg::rendering {
    class Renderer2DLowLevel {
    public:
        static Ref <Renderer2DLowLevel> create(GraphicsAPI& graphicsAPI, Renderer2D::Settings settings, VertexBufferLayout layout);

        virtual void begin(Camera2D camera, Ref <Shader> shader) = 0;

        virtual void
        drawCall(Renderer2D::QuadVertex* quadVertexData, Renderer2D::IndicesData* indicesData, u32 quadCount,
                 Ref <rendering::Texture>* textures, u32 textureCount) = 0;

        virtual void imguiDebug() = 0;

        virtual void end() = 0;
    };
}


#endif //EUGINE_RENDERER2DLOWLEVEL_H

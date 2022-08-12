//
// Created by aiden on 8/12/22.
//

#ifndef EUGINE_RENDERERLOWLEVEL_H
#define EUGINE_RENDERERLOWLEVEL_H

#include "eugine/rendering/Renderer2DLowLevel.h"
#include "eugine/rendering/Renderer2D.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "UniformBuffer.h"
#include "Shader.h"
#include "Texture.h"

namespace eg::rendering::GLWrapper {
    class Renderer2DLowLevel : public rendering::Renderer2DLowLevel {
    public:
        Renderer2DLowLevel(rendering::Renderer2D::Settings settings, eg::rendering::VertexBufferLayout vboLayout);
        ~Renderer2DLowLevel();

        void begin(Camera2D camera, Ref<rendering::Shader> shader) override;

        void drawCall(rendering::Renderer2D::QuadVertex* quadVertexData, rendering::Renderer2D::IndicesData* indicesData, u32 quadCount,
                      Ref <rendering::Texture>* textures, u32 textureCount) override;

        void imguiDebug() override;

        void end() override;

    private:
        Renderer2D::Settings m_settings;

        Ref<Shader> m_currentShader = nullptr;

        Ref<UniformBuffer> m_uniformBuffer;
        Ref<VertexBuffer> m_vertexBuffer;
        Ref<IndexBuffer> m_indexBuffer;
    };
}


#endif //EUGINE_RENDERER2DLOWLEVEL_H

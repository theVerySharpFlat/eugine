//
// Created by aiden on 5/6/22.
//

#ifndef EUGINE_RENDERER2D_H
#define EUGINE_RENDERER2D_H

#include "GraphicsAPI.h"
#include "Camera2D.h"
#include "glm/glm.hpp"
#include "Texture.h"
#include "LowLevelRenderer.h"

namespace eg::rendering {
    class Renderer2D {
    public:
        struct Settings {
            u32 maxQuads;
        };
        static Ref<Renderer2D> create(const Settings& settings);

        void begin(Ref<Camera2D> camera);
        void submitQuad(glm::vec2 position, glm::vec2 dimensions, glm::vec3 color, Ref<Texture> texture);
        void end();

        ~Renderer2D();
    private:
        Renderer2D(const Settings& settings);

        struct RenderData {
            float* vertices;
            u32 verticesByteSize;
            u32 maxVertexCount;
            u32* indices;
            u32 indicesByteSize;
            u32 maxIndexCount;

            Ref<Shader> shader;

            Ref<VertexBuffer> vbo;
            Ref<IndexBuffer> ibo;
            Ref<VertexArray> vao;
        };
        RenderData m_renderData = {};

        struct FrameData {
            float* vertexDataPtr;
            u32* indexDataptr;

            u32 quadCount;
            u32 vertexCount;
            u32 indexCount;
        };
        FrameData m_frameData = {};


        Ref<LowLevelRenderer> m_lowLevelRenderer;
    };
};

#endif //EUGINE_RENDERER2D_H

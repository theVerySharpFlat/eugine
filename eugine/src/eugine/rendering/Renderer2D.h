//
// Created by aiden on 5/6/22.
//

#ifndef EUGINE_RENDERER2D_H
#define EUGINE_RENDERER2D_H

#include "GraphicsAPI.h"
#include "Camera2D.h"
#include "eugine/rendering/Camera.h"
#include "glm/glm.hpp"
#include "Texture.h"
#include "LowLevelRenderer.h"

namespace eg::rendering {
    class Renderer2D {
    public:
        struct Settings {
            u32 maxQuads;
            i32 maxTextures;
        };
        static Ref<Renderer2D> create(const Settings& settings);

        void begin(Ref<Camera2D> camera);
        void submitQuad(glm::vec2 position, glm::vec2 dimensions, float rotation, glm::vec4 color, const Ref<Texture>& texture);
        void flush();
        void end();
        
        void imguiDbg();

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

            u32 maxTextures;
            Ref<Texture>* textures;

        };
        RenderData m_renderData = {};

        struct BatchData {
            float* vertexDataPtr;
            u32* indexDataptr;

            u32 texIndex;

            u32 quadCount;
            u32 vertexCount;
            u32 indexCount;
            
            Ref<Camera2D> camera;
        };
        BatchData m_batchData = {};
        
        struct FrameData {
            u32 quadCount;
            u32 vertexCount;
            u32 indexCount;
            u32 batchCount;
        };
        FrameData m_frameData = {};
        FrameData m_prevFrameData = {};

        Ref<LowLevelRenderer> m_lowLevelRenderer;
        
        void batchReset();
    };
};

#endif //EUGINE_RENDERER2D_H

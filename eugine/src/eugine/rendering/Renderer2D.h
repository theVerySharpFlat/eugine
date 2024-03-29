//
// Created by aiden on 5/6/22.
//

#ifndef EUGINE_RENDERER2D_H
#define EUGINE_RENDERER2D_H

#include "GraphicsAPI.h"
#include "Camera2D.h"
#include "Texture.h"

namespace eg::rendering {
    class Renderer2DLowLevel;
    class Shader;

    class Renderer2D {
    public:
        struct Settings {
            u16 maxQuadsPerBatch;
            u16 maxTextures = 0; // 0 = maximum supported samplers per shader stage
            bool textureRedundancyCheck;
        };
        Renderer2D(GraphicsAPI& graphicsAPI, Settings settings);
        ~Renderer2D();

        void begin(Camera2D& camera);
        void end();

        void imguiDebug();

        struct Quad {
            glm::vec2 center;
            glm::vec2 dimensions;

            Ref<Texture> texture;
            glm::vec2 textureBottomLeft = {0.0f, 0.0f};
            glm::vec2 textureTopRight = {1.0f, 1.0f};

            glm::vec4 color;
            float fragmentAlphaMultiplier;

            float rotation;
        };
        void queueQuad(Quad quad);

        void flush();

        struct QuadVertex {
            glm::vec2 position;
            glm::vec2 texCoord;
            glm::vec4 color;
            float fragmentAlphaBlendFactor;
            u32 textureIndex;
        };

        struct IndicesData {
            u16 indices[6]{};

            explicit IndicesData(u16 quadIndex) {
                indices[0] = 0 + 4 * quadIndex;
                indices[1] = 1 + 4 * quadIndex;
                indices[2] = 2 + 4 * quadIndex;
                indices[3] = 2 + 4 * quadIndex;
                indices[4] = 3 + 4 * quadIndex;
                indices[5] = 0 + 4 * quadIndex;
            }
        };

    private:
        GraphicsAPI& m_graphicsAPI;

        Settings m_settings;

        struct BatchData {
            u16 currentQuadIndex = 0;
            u16 currentTextureIndex = 1;
        };
        BatchData m_batchData{};

        QuadVertex* m_quadVertexData = nullptr;
        IndicesData* m_indexData = nullptr;
        Ref<rendering::Texture>* m_textures = nullptr;

        Ref<Renderer2DLowLevel> m_lowLevelRenderer = nullptr;
        Ref<Shader> m_shader = nullptr;
        Ref<Texture> m_defaultTexture = nullptr;

        struct FrameStats {
            u32 batchCount = 0;
            u32 quadCount = 0;
        };
        FrameStats m_frameStats = {};
    };
};

#endif //EUGINE_RENDERER2D_H

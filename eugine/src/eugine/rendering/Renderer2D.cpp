#include <iostream>
#include "Renderer2D.h"
#include "Renderer2DLowLevel.h"

#include "eugine/rendering/GraphicsAPI.h"
#include "imgui/imgui.h"

#include "incbin.h"

INCTXT(QuadShaderFragSource, "eugine/rendering/shaders/Renderer2D/quadShader.frag");
INCTXT(QuadShaderVertSource, "eugine/rendering/shaders/Renderer2D/quadShader.vert");
INCBIN(WhiteTexturePNG, "eugine/rendering/textures/WhiteTexture.png");

namespace eg::rendering {
    Renderer2D::Renderer2D(GraphicsAPI& graphicsAPI, Renderer2D::Settings settings) : m_graphicsAPI(graphicsAPI),
                                                                                           m_settings(settings) {
        if (m_settings.maxTextures == 0) {
            m_settings.maxTextures = m_graphicsAPI.getMaxTexturesPerShader();
        }

        m_quadVertexData = (QuadVertex*) malloc(sizeof(QuadVertex) * 4 * settings.maxQuadsPerBatch);
        m_indexData = (IndicesData*) malloc(sizeof(IndicesData) * settings.maxQuadsPerBatch);
        m_textures = new Ref<Texture>[m_settings.maxTextures];

        VertexBufferLayout vboLayout(5);
        vboLayout.setAttribute(0, {SHDR_VEC2, 1});
        vboLayout.setAttribute(1, {SHDR_VEC2, 1});
        vboLayout.setAttribute(2, {SHDR_VEC4, 1});
        vboLayout.setAttribute(3, {SHDR_FLOAT, 1});
        vboLayout.setAttribute(4, {SHDR_UINT, 1});

        m_lowLevelRenderer = Renderer2DLowLevel::create(graphicsAPI, m_settings, vboLayout);

        Shader::ShaderProgramSource shaderProgramSource{
                {
                        "eugine/rendering/shaders/quadShader.vert",
                        gQuadShaderVertSourceData,
                        gQuadShaderVertSourceSize - 1
                },
                {
                        "eugine/rendering/shaders/quadShader.frag",
                        gQuadShaderFragSourceData,
                        gQuadShaderFragSourceSize - 1
                }
        };


        std::initializer_list<ShaderBindingDescription> shaderBindingDescriptions = {
                {
                        "PerFrameUBO",
                        SHADER_BINDING_TYPE_UNIFORM_BUFFER,
                        1
                },
                {
                        "samplers",
                        SHADER_BINDING_TYPE_SAMPLER_ARRAY,
                        128
                }
        };

        std::initializer_list<ShaderUniform> shaderUniforms = {};

        m_shader = Shader::create(shaderProgramSource, {shaderUniforms, shaderBindingDescriptions}, vboLayout);

        m_defaultTexture = Texture::create(gWhiteTexturePNGData, gWhiteTexturePNGSize, "WhiteTexture.png");
        m_textures[0] = m_defaultTexture;
    };

    static glm::vec2 rotate2D(const glm::vec2& point, const glm::vec2& center, const float& angle) {
        float cosVal = glm::cos(angle);
        float sinVal = glm::sin(angle);

        return {
                (point.x - center.x) * cosVal - (point.y - center.y) * sinVal + center.x,
                (point.x - center.x) * sinVal + (point.y - center.y) * cosVal + center.y
        };
    }

    void Renderer2D::queueQuad(Renderer2D::Quad quad) {
        // trace("max textures: {}", m_settings.maxTextures);
        if (m_batchData.currentQuadIndex >= m_settings.maxQuadsPerBatch ||
            m_batchData.currentTextureIndex >= m_settings.maxTextures) {
            flush();
        }

        QuadVertex baseVertex{};
        baseVertex.color = quad.color;
        baseVertex.fragmentAlphaBlendFactor = quad.fragmentAlphaMultiplier;
        bool redundantTexture = false;
        if(quad.texture != nullptr) {
            baseVertex.textureIndex = m_batchData.currentTextureIndex;
            if(m_settings.textureRedundancyCheck) {
                for(u32 i = 0; i < m_batchData.currentTextureIndex; i++) {
                    if(m_textures[i] == quad.texture) {
                        baseVertex.textureIndex = i;
                        redundantTexture = true;
                        break;
                    }
                }
            }
            m_textures[m_batchData.currentTextureIndex] = quad.texture;//quad.texture;
        } else {
            baseVertex.textureIndex = 0;
        }

        bool topLeftTexCoords = getPreferredGraphicsAPI() == EG_API_VK;

        QuadVertex bottomLeftVertex = baseVertex;
        bottomLeftVertex.position = rotate2D(
                {quad.center.x - quad.dimensions.x / 2, quad.center.y - quad.dimensions.y / 2},
                quad.center,
                quad.rotation
                );
        if(topLeftTexCoords)
          bottomLeftVertex.texCoord = {quad.textureBottomLeft.x, quad.textureBottomLeft.y + 1.0f};
        else
          bottomLeftVertex.texCoord = {quad.textureBottomLeft.x, quad.textureBottomLeft.y};

        QuadVertex bottomRightVertex = baseVertex;
        bottomRightVertex.position = rotate2D(
                {quad.center.x + quad.dimensions.x / 2, quad.center.y - quad.dimensions.y / 2},
                quad.center,
                quad.rotation
                );
        if(topLeftTexCoords)
          bottomRightVertex.texCoord = {quad.textureTopRight.x, quad.textureBottomLeft.y + 1.0f};
        else
          bottomRightVertex.texCoord = {quad.textureTopRight.x, quad.textureBottomLeft.y};


        QuadVertex topRightVertex = baseVertex;
        topRightVertex.position = rotate2D(
                {quad.center.x + quad.dimensions.x / 2, quad.center.y + quad.dimensions.y / 2},
                quad.center,
                quad.rotation
                );
        if(topLeftTexCoords)
          topRightVertex.texCoord = {quad.textureTopRight.x, -quad.textureTopRight.y + 1.0f};
        else
          topRightVertex.texCoord = {quad.textureTopRight.x, quad.textureTopRight.y};

        QuadVertex topLeftVertex = baseVertex;
        topLeftVertex.position = rotate2D(
                {quad.center.x - quad.dimensions.x / 2, quad.center.y + quad.dimensions.y / 2},
                quad.center,
                quad.rotation
                );
        if(topLeftTexCoords)
          topLeftVertex.texCoord = {quad.textureBottomLeft.x, -quad.textureTopRight.y + 1.0f};
        else
          topLeftVertex.texCoord = {quad.textureBottomLeft.x, quad.textureTopRight.y};

        m_quadVertexData[m_batchData.currentQuadIndex * 4 + 0] = bottomLeftVertex;
        m_quadVertexData[m_batchData.currentQuadIndex * 4 + 1] = bottomRightVertex;
        m_quadVertexData[m_batchData.currentQuadIndex * 4 + 2] = topRightVertex;
        m_quadVertexData[m_batchData.currentQuadIndex * 4 + 3] = topLeftVertex;

        m_indexData[m_batchData.currentQuadIndex] = IndicesData(m_batchData.currentQuadIndex);

        m_batchData.currentQuadIndex++;
        m_frameStats.quadCount++;

        if(quad.texture != nullptr && !redundantTexture) {
            m_batchData.currentTextureIndex++;
        }
    }

    void Renderer2D::flush() {
        m_lowLevelRenderer->drawCall(m_quadVertexData, m_indexData, m_batchData.currentQuadIndex, m_textures,
                                     m_batchData.currentTextureIndex);
        m_batchData.currentQuadIndex = 0;
        m_batchData.currentTextureIndex = 1;

        m_frameStats.batchCount++;
    }

    void Renderer2D::begin(Camera2D& camera) {
        m_frameStats = {};
        m_lowLevelRenderer->begin(camera, m_shader);
    }

    void Renderer2D::end() {
        if (m_batchData.currentQuadIndex != 0) {
            flush();
        }
        m_lowLevelRenderer->end();
    }

    void Renderer2D::imguiDebug() {
        m_lowLevelRenderer->imguiDebug();
        ImGui::Begin("Renderer2D");

        if(ImGui::TreeNode("Renderer2D::Settings")) {
            ImGui::Text("Max Textures Per Batch: %d", m_settings.maxTextures);
            ImGui::Text("Max Quads Per Batch: %d", m_settings.maxQuadsPerBatch);
            ImGui::Checkbox("Texture redundancy checking", &m_settings.textureRedundancyCheck);
            ImGui::TreePop();
        }

        if(ImGui::TreeNode("Renderer2D::FrameStats")) {
            ImGui::Text("Quad count: %d", m_frameStats.quadCount);
            ImGui::Text("Batch Count: %d", m_frameStats.batchCount);
            ImGui::TreePop();
        }

        ImGui::End();
    }

    Renderer2D::~Renderer2D() {
        free(m_quadVertexData);
        free(m_indexData);
        for(u32 i = 0; i < m_settings.maxTextures; i++) {
            m_textures[i] = nullptr;
        }
        delete[] m_textures;
    }
}

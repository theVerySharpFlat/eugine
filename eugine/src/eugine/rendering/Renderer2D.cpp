#include <iostream>
#include "Renderer2D.h"
#include "Renderer2DLowLevel.h"

#include "incbin.h"

INCTXT(QuadShaderFragSource, "eugine/rendering/shaders/Renderer2D/quadShader.frag");
INCTXT(QuadShaderVertSource, "eugine/rendering/shaders/Renderer2D/quadShader.vert");


namespace eg::rendering {
    Renderer2D::Renderer2D(GraphicsAPI& graphicsAPI, Renderer2D::Settings settings) : m_graphicsAPI(graphicsAPI),
                                                                                           m_settings(settings) {
        if (m_settings.maxTextures == 0)
            m_settings.maxTextures = m_graphicsAPI.getMaxTexturesPerShader();
        trace("in Renderer2D maxTextures: {}", m_settings.maxTextures);

        m_quadVertexData = (QuadVertex*) malloc(sizeof(QuadVertex) * 4 * settings.maxQuadsPerBatch);
        m_indexData = (IndicesData*) malloc(sizeof(IndicesData) * settings.maxQuadsPerBatch);
        m_textures = new Ref<Texture>[m_settings.maxTextures];

        m_lowLevelRenderer = Renderer2DLowLevel::create(graphicsAPI, m_settings);

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

        std::cout << "vertex shader:\n" << std::string(gQuadShaderVertSourceData, gQuadShaderVertSourceSize)
                  << std::endl;

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

        VertexBufferLayout vboLayout(5);
        vboLayout.setAttribute(0, {SHDR_VEC2, 1});
        vboLayout.setAttribute(1, {SHDR_VEC2, 1});
        vboLayout.setAttribute(2, {SHDR_VEC4, 1});
        vboLayout.setAttribute(3, {SHDR_FLOAT, 1});
        vboLayout.setAttribute(4, {SHDR_UINT, 1});


        m_shader = Shader::create(shaderProgramSource, {{}, shaderBindingDescriptions}, vboLayout);
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
        baseVertex.textureIndex = m_batchData.currentTextureIndex;

        QuadVertex bottomLeftVertex = baseVertex;
        bottomLeftVertex.position = rotate2D(
                {quad.center.x - quad.dimensions.x / 2, quad.center.y - quad.dimensions.y / 2},
                quad.center,
                quad.rotation
                );
        bottomLeftVertex.texCoord = {1.0f, 0.0f};

        QuadVertex bottomRightVertex = baseVertex;
        bottomRightVertex.position = rotate2D(
                {quad.center.x + quad.dimensions.x / 2, quad.center.y - quad.dimensions.y / 2},
                quad.center,
                quad.rotation
                );
        bottomRightVertex.texCoord = {0.0f, 0.0f};

        QuadVertex topRightVertex = baseVertex;
        topRightVertex.position = rotate2D(
                {quad.center.x + quad.dimensions.x / 2 + 100, quad.center.y + quad.dimensions.y / 2},
                quad.center,
                quad.rotation
                );
        topRightVertex.texCoord = {0.0f, 1.0f};

        QuadVertex topLeftVertex = baseVertex;
        topLeftVertex.position = rotate2D(
                {quad.center.x - quad.dimensions.x / 2, quad.center.y + quad.dimensions.y / 2},
                quad.center,
                quad.rotation
                );
        topLeftVertex.texCoord = {1.0f, 1.0f};

        m_quadVertexData[m_batchData.currentQuadIndex * 4 + 0] = bottomLeftVertex;
        m_quadVertexData[m_batchData.currentQuadIndex * 4 + 1] = bottomRightVertex;
        m_quadVertexData[m_batchData.currentQuadIndex * 4 + 2] = topRightVertex;
        m_quadVertexData[m_batchData.currentQuadIndex * 4 + 3] = topLeftVertex;

        m_indexData[m_batchData.currentQuadIndex] = IndicesData(m_batchData.currentQuadIndex);
        m_textures[m_batchData.currentTextureIndex] = quad.texture;

        m_batchData.currentQuadIndex++;
        m_batchData.currentTextureIndex++;
    }

    void Renderer2D::flush() {
        m_lowLevelRenderer->drawCall(m_quadVertexData, m_indexData, m_batchData.currentQuadIndex, m_textures,
                                     m_batchData.currentTextureIndex);
        m_batchData.currentQuadIndex = 0;
        m_batchData.currentTextureIndex = 0;
    }

    void Renderer2D::begin(Camera2D& camera) {
        m_lowLevelRenderer->begin(camera, m_shader);
    }

    void Renderer2D::end() {
        if (m_batchData.currentQuadIndex != 0) {
            flush();
        }
        m_lowLevelRenderer->end();
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
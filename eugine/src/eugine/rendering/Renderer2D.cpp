//
// Created by aiden on 5/6/22.
//

#include "Renderer2D.h"
#include <incbin.h>

INCTXT(VertexShader, "eugine/rendering/shaders/Renderer2D/shader.vert");
INCTXT(FragmentShader, "eugine/rendering/shaders/Renderer2D/shader.frag");

namespace eg::rendering {
    Ref <Renderer2D> Renderer2D::create(const Settings& settings) {
        return Ref<Renderer2D>(new Renderer2D(settings));
    }

    void Renderer2D::begin(eg::Ref<Camera2D> camera) {
        m_renderData.shader->setMat4("projxview", camera->getProjectionTimesView());
        m_frameData.vertexDataPtr = m_renderData.vertices;
        m_frameData.indexDataptr = m_renderData.indices;
        m_frameData.quadCount = 0;
        m_frameData.vertexCount = 0;
    }

    void Renderer2D::submitQuad(glm::vec2 position, glm::vec2 dimensions, Ref<Texture> texture) {
        m_frameData.vertexDataPtr[0] = position.x - dimensions.x / 2; // top left
        m_frameData.vertexDataPtr[1] = position.y + dimensions.y / 2;
        m_frameData.vertexDataPtr[2] = 1.0f;
        m_frameData.vertexDataPtr[3] = 1.0f;
        m_frameData.vertexDataPtr[4] = 1.0f;
        m_frameData.vertexDataPtr[5] = 1.0f;
        m_frameData.vertexDataPtr += 6;

        m_frameData.vertexDataPtr[0] = position.x - dimensions.x / 2; // bottom left
        m_frameData.vertexDataPtr[1] = position.y - dimensions.y / 2;
        m_frameData.vertexDataPtr[2] = 1.0f;
        m_frameData.vertexDataPtr[3] = 1.0f;
        m_frameData.vertexDataPtr[4] = 1.0f;
        m_frameData.vertexDataPtr[5] = 1.0f;
        m_frameData.vertexDataPtr += 6;

        m_frameData.vertexDataPtr[0] = position.x + dimensions.x / 2; // bottom right
        m_frameData.vertexDataPtr[1] = position.y - dimensions.y / 2;
        m_frameData.vertexDataPtr[2] = 1.0f;
        m_frameData.vertexDataPtr[3] = 1.0f;
        m_frameData.vertexDataPtr[4] = 1.0f;
        m_frameData.vertexDataPtr[5] = 1.0f;
        m_frameData.vertexDataPtr += 6;

        m_frameData.vertexDataPtr[0] = position.x + dimensions.x / 2; // top right
        m_frameData.vertexDataPtr[1] = position.y + dimensions.y / 2;
        m_frameData.vertexDataPtr[2] = 1.0f;
        m_frameData.vertexDataPtr[3] = 1.0f;
        m_frameData.vertexDataPtr[4] = 1.0f;
        m_frameData.vertexDataPtr[5] = 1.0f;
        m_frameData.vertexDataPtr += 6;

        m_frameData.indexDataptr[m_frameData.vertexCount + 0] = m_frameData.vertexCount + 0; // top left
        m_frameData.indexDataptr[m_frameData.vertexCount + 1] = m_frameData.vertexCount + 1; // bottom left
        m_frameData.indexDataptr[m_frameData.vertexCount + 2] = m_frameData.vertexCount + 2; // bottom right
        m_frameData.indexDataptr[m_frameData.vertexCount + 3] = m_frameData.vertexCount + 0; // top left
        m_frameData.indexDataptr[m_frameData.vertexCount + 4] = m_frameData.vertexCount + 3; // top right
        m_frameData.indexDataptr[m_frameData.vertexCount + 5] = m_frameData.vertexCount + 2; // bottom right
        m_frameData.indexDataptr += 6;

        m_frameData.quadCount += 1;
        m_frameData.vertexCount += 4;
    }

    void Renderer2D::end() {
        m_renderData.shader->bind();
        m_renderData.vbo->setData(m_renderData.vertices, ((uint8_t*)m_frameData.vertexDataPtr - (uint8_t*)m_renderData.vertices));
        m_renderData.ibo->setData(m_renderData.indices, ((uint8_t*)m_frameData.indexDataptr - (uint8_t*)m_renderData.indices));
        m_renderData.vao->setBuffer(*m_renderData.vbo);
        m_renderData.ibo->setElementCount(m_frameData.indexDataptr - m_renderData.indices);
        trace("element count: {}", m_frameData.indexDataptr - m_renderData.indices);
        m_lowLevelRenderer->drawIndexed(m_renderData.vao, m_renderData.ibo, m_renderData.shader);
    }

    Renderer2D::~Renderer2D() {
        free(m_renderData.vertices);
        free(m_renderData.indices);
    }

    Renderer2D::Renderer2D(const Settings& settings) {
        m_lowLevelRenderer = LowLevelRenderer::create();

        VertexBufferLayout layout = {
                { // position
                        SHDR_FLOAT,
                        2
                },
                { // color
                        SHDR_FLOAT,
                        3
                },
                {
                        SHDR_FLOAT,
                        1
                }
        };

        m_renderData.maxVertexCount = settings.maxQuads * 4;
        m_renderData.verticesByteSize = layout.getStride() * m_renderData.maxVertexCount;
        m_renderData.vertices = (float*)malloc(m_renderData.verticesByteSize);

        m_renderData.maxIndexCount = settings.maxQuads * 6;
        m_renderData.indicesByteSize = sizeof(u32) * m_renderData.maxIndexCount;
        m_renderData.indices  = (u32*)malloc(m_renderData.indicesByteSize);

        m_renderData.vbo = VertexBuffer::create(m_renderData.vertices, m_renderData.verticesByteSize, layout);
        m_renderData.ibo = IndexBuffer::create(m_renderData.indices, m_renderData.indicesByteSize);
        m_renderData.vao = VertexArray::create();

        m_renderData.vao->setBuffer(*m_renderData.vbo);

        m_renderData.shader = Shader::create({
                                                     {
                                                         gVertexShaderData,
                                                         gVertexShaderSize
                                                     },
                                                     {
                                                         gFragmentShaderData,
                                                         gFragmentShaderSize
                                                     }
        });
    }
}
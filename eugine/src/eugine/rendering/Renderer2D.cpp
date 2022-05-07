//
// Created by aiden on 5/6/22.
//

#include "Renderer2D.h"

namespace eg::rendering {
    Ref <Renderer2D> Renderer2D::create(const Settings& settings) {
        return Ref<Renderer2D>(new Renderer2D(settings));
    }

    void Renderer2D::begin(eg::Ref<Camera2D> camera) {

    }

    void Renderer2D::submitQuad(glm::vec2 position, glm::vec2 dimensions, Ref<Texture> texture) {

    }

    void Renderer2D::end() {

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
                        SHDR_UINT,
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
    }
}
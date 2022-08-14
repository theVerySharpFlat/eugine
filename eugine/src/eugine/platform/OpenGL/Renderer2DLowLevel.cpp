//
// Created by aiden on 8/12/22.
//

#include "Renderer2DLowLevel.h"

#include "error.h"

namespace eg::rendering::GLWrapper {
    Renderer2DLowLevel::Renderer2DLowLevel(rendering::Renderer2D::Settings settings,
                                           eg::rendering::VertexBufferLayout vboLayout) : m_settings(
            settings) {
        m_uniformBuffer = Ref<GLWrapper::UniformBuffer>(
                new GLWrapper::UniformBuffer(sizeof(glm::mat4), VertexBuffer::VB_USAGE_HINT_DYNAMIC));
        m_vertexBuffer = Ref<GLWrapper::VertexBuffer>(
                new GLWrapper::VertexBuffer(nullptr, m_settings.maxQuadsPerBatch * 4 * sizeof(Renderer2D::QuadVertex),
                                            VertexBuffer::VB_USAGE_HINT_DYNAMIC, vboLayout));
        m_indexBuffer = Ref<GLWrapper::IndexBuffer>(
                new GLWrapper::IndexBuffer(nullptr, sizeof(Renderer2D::IndicesData) * m_settings.maxQuadsPerBatch,
                                           VertexBuffer::VB_USAGE_HINT_DYNAMIC));
    }

    Renderer2DLowLevel::~Renderer2DLowLevel() {

    }

    void Renderer2DLowLevel::begin(Camera2D camera, Ref <rendering::Shader> shader) {
        m_currentShader = std::dynamic_pointer_cast<GLWrapper::Shader>(shader);

        m_currentShader->bind();
        glm::mat4 projxview = camera.getProjectionTimesView();
        m_currentShader->setMat4("projxview", projxview);
    }

    void Renderer2DLowLevel::drawCall(Renderer2D::QuadVertex* quadVertexData, Renderer2D::IndicesData* indicesData,
                                      u32 quadCount, Ref <rendering::Texture>* textures, u32 textureCount) {
        m_vertexBuffer->bind();
        m_vertexBuffer->setData(quadVertexData, sizeof(Renderer2D::QuadVertex) * 4 * quadCount);

        m_indexBuffer->bind();
        m_indexBuffer->setData((u16*)indicesData, 6 * quadCount);

        m_currentShader->setTextureArray("samplers", textures, textureCount);

        GLCall(glDrawElements(GL_TRIANGLES, quadCount * 6, GL_UNSIGNED_SHORT, NULL));
    }

    void Renderer2DLowLevel::imguiDebug() {

    }

    void Renderer2DLowLevel::end() {

    }
}
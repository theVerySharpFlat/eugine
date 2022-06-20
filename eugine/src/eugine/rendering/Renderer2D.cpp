//
// Created by aiden on 5/6/22.
//

#include "Renderer2D.h"
#include "glm/ext/matrix_transform.hpp"
#include <incbin.h>
#include <imgui/imgui.h>
#include <iostream>

INCTXT(VertexShader, "eugine/rendering/shaders/Renderer2D/shader.vert");
INCTXT(FragmentShader, "eugine/rendering/shaders/Renderer2D/shader.frag");

namespace eg::rendering {
    Ref <Renderer2D> Renderer2D::create(const Settings &settings) {
        return Ref<Renderer2D>(new Renderer2D(settings));
    }

    void Renderer2D::begin(eg::Ref<Camera2D> camera) {
        m_batchData.camera = camera;
        m_renderData.shader->setMat4("projxview", m_batchData.camera->getProjectionTimesView());

        m_frameData.batchCount = 0;
        m_frameData.indexCount = 0;
        m_frameData.vertexCount = 0;
        m_frameData.quadCount = 0;
        batchReset();
    }

    void Renderer2D::batchReset() {
        m_batchData.vertexDataPtr = m_renderData.vertices;
        m_batchData.indexDataptr = m_renderData.indices;
        m_batchData.quadCount = 0;
        m_batchData.vertexCount = 0;
        m_batchData.indexCount = 0;
        m_batchData.texIndex = 0;
    }

    void Renderer2D::submitQuad(glm::vec2 position, glm::vec2 dimensions, float rotation, glm::vec4 color, const Ref<Texture>& texture) {
        if ((((u8 *) m_batchData.vertexDataPtr - (u8 *) m_renderData.vertices) /
             m_renderData.vbo->getLayout().getStride()) >= m_renderData.maxVertexCount) {
            flush();
        }

        if(m_batchData.texIndex >= m_renderData.maxTextures) {
            flush();
        }

        // check if texture is already in the list
        // -1.0f means solid color
        float index = -1.0f;
        if(texture != nullptr) {
            for(int i = 0; i < m_batchData.texIndex; i++) {
                if(*m_renderData.textures[i] == *texture) {
                    index = (float)i;
                }
            }

            if(index == -1.0f) {
                m_renderData.textures[m_batchData.texIndex] = texture;
                index = (float)m_batchData.texIndex;
                m_batchData.texIndex++;
            }
        }

        glm::mat2 transform{
            cosf(rotation), -sinf(rotation),
            sin(rotation), cos(rotation)
        };

        glm::vec2 vertex = {
            -dimensions.x / 2,
            +dimensions.y / 2
        };

        vertex = transform * vertex + position;
        m_batchData.vertexDataPtr[0] = vertex.x; // top left
        m_batchData.vertexDataPtr[1] = vertex.y;
        m_batchData.vertexDataPtr[2] = color.r;
        m_batchData.vertexDataPtr[3] = color.g;
        m_batchData.vertexDataPtr[4] = color.b;
        m_batchData.vertexDataPtr[5] = color.a;
        m_batchData.vertexDataPtr[6] = 0.0f;
        m_batchData.vertexDataPtr[7] = 1.0f;
        m_batchData.vertexDataPtr[8] = (float) index;
        m_batchData.vertexDataPtr += 9;


        vertex = {
            -dimensions.x / 2,
            -dimensions.y / 2
        };
        vertex = transform * vertex + position;
        m_batchData.vertexDataPtr[0] = vertex.x; // bottom left
        m_batchData.vertexDataPtr[1] = vertex.y;
        m_batchData.vertexDataPtr[2] = color.r;
        m_batchData.vertexDataPtr[3] = color.g;
        m_batchData.vertexDataPtr[4] = color.b;
        m_batchData.vertexDataPtr[5] = color.a;
        m_batchData.vertexDataPtr[6] = 0.0f;
        m_batchData.vertexDataPtr[7] = 0.0f;
        m_batchData.vertexDataPtr[8] = (float) index;
        m_batchData.vertexDataPtr += 9;

        vertex = {
            +dimensions.x / 2,
            -dimensions.y / 2
        };
        vertex = transform * vertex + position;
        m_batchData.vertexDataPtr[0] = vertex.x; // bottom right
        m_batchData.vertexDataPtr[1] = vertex.y;
        m_batchData.vertexDataPtr[2] = color.r;
        m_batchData.vertexDataPtr[3] = color.g;
        m_batchData.vertexDataPtr[4] = color.b;
        m_batchData.vertexDataPtr[5] = color.a;
        m_batchData.vertexDataPtr[6] = 1.0f;
        m_batchData.vertexDataPtr[7] = 0.0f;
        m_batchData.vertexDataPtr[8] = (float) index;
        m_batchData.vertexDataPtr += 9;


        vertex = {
            +dimensions.x / 2,
            +dimensions.y / 2
        };
        vertex = transform * vertex + position;
        m_batchData.vertexDataPtr[0] = vertex.x; // top right
        m_batchData.vertexDataPtr[1] = vertex.y;
        m_batchData.vertexDataPtr[2] = color.r;
        m_batchData.vertexDataPtr[3] = color.g;
        m_batchData.vertexDataPtr[4] = color.b;
        m_batchData.vertexDataPtr[5] = color.a;
        m_batchData.vertexDataPtr[6] = 1.0f;
        m_batchData.vertexDataPtr[7] = 1.0f;
        m_batchData.vertexDataPtr[8] = (float) index;
        m_batchData.vertexDataPtr += 9;

        m_batchData.indexDataptr[0] = m_batchData.vertexCount + 0; // top left
        m_batchData.indexDataptr[1] = m_batchData.vertexCount + 1; // bottom left
        m_batchData.indexDataptr[2] = m_batchData.vertexCount + 2; // bottom right
        m_batchData.indexDataptr[3] = m_batchData.vertexCount + 0; // top left
        m_batchData.indexDataptr[4] = m_batchData.vertexCount + 3; // top right
        m_batchData.indexDataptr[5] = m_batchData.vertexCount + 2; // bottom right
        m_batchData.indexDataptr += 6;

        m_batchData.quadCount += 1;
        m_batchData.vertexCount += 4;
        m_batchData.indexCount += 6;
    }

    void Renderer2D::end() {
        if (m_batchData.quadCount)
            flush();

        m_prevFrameData = m_frameData;
    }

    void Renderer2D::flush() {
        m_renderData.shader->bind();
        
        // set samplers
        i32* samplers = (i32*)alloca(sizeof(i32) * m_renderData.maxTextures);
        for(int i = 0; i < m_renderData.maxTextures; i++) {
            samplers[i] = i;
        }
        m_renderData.shader->setIntArray("samplers", samplers, m_renderData.maxTextures);
        
        // set textures
        for(int i = 0; i < m_batchData.texIndex; i++) {
            m_renderData.textures[i]->bind(i);
        }

        // load vertices and indices into buffers
        m_renderData.vbo->setData(m_renderData.vertices,
                                  ((u8*) m_batchData.vertexDataPtr - (u8*) m_renderData.vertices));
        m_renderData.ibo->setData(m_renderData.indices,
                                  ((u8*) m_batchData.indexDataptr - (u8*) m_renderData.indices));
        
        m_renderData.vao->setBuffer(*m_renderData.vbo);
        m_renderData.ibo->setElementCount(m_batchData.indexDataptr - m_renderData.indices);

        m_lowLevelRenderer->drawIndexed(m_renderData.vao, m_renderData.ibo, m_renderData.shader);

        m_frameData.batchCount++;
        m_frameData.indexCount += m_batchData.indexCount;
        m_frameData.quadCount += m_batchData.quadCount;
        m_frameData.vertexCount += m_batchData.vertexCount;

        batchReset();
    }

    Renderer2D::~Renderer2D() {
        free(m_renderData.vertices);
        free(m_renderData.indices);
        delete[] m_renderData.textures;
    }

    Renderer2D::Renderer2D(const Settings &settings) {
        m_lowLevelRenderer = LowLevelRenderer::create();

        VertexBufferLayout layout = {
                { // position
                        SHDR_FLOAT,
                        2
                },
                { // color
                        SHDR_FLOAT,
                        4
                },
                { // texture coords
                        SHDR_FLOAT,
                        2
                },
                { // texture index
                        SHDR_FLOAT,
                        1
                }
        };

        m_renderData.maxVertexCount = settings.maxQuads * 4;
        m_renderData.verticesByteSize = layout.getStride() * m_renderData.maxVertexCount;
        m_renderData.vertices = (float*)malloc(m_renderData.verticesByteSize);

        m_renderData.maxIndexCount = settings.maxQuads * 6;
        m_renderData.indicesByteSize = sizeof(u32) * m_renderData.maxIndexCount;
        m_renderData.indices = (u32*)malloc(m_renderData.indicesByteSize);

        m_renderData.vbo = VertexBuffer::create(m_renderData.vertices, m_renderData.verticesByteSize, layout);
        m_renderData.ibo = IndexBuffer::create(m_renderData.indices, m_renderData.indicesByteSize);
        m_renderData.vao = VertexArray::create();

        m_renderData.vao->setBuffer(*m_renderData.vbo);

        m_renderData.shader = Shader::create({
                                                     {
                                                             "ashadervert",
                                                             gVertexShaderData,
                                                             gVertexShaderSize
                                                     },
                                                     {
                                                             "ashaderfrag",
                                                             gFragmentShaderData,
                                                             gFragmentShaderSize
                                                     }
                                             });

        m_renderData.maxTextures = settings.maxTextures;
        m_renderData.textures = new Ref<Texture>[settings.maxTextures];
    }

    void Renderer2D::imguiDbg() {
        ImGui::Begin("renderer");
        ImGui::Text("%d batches", m_prevFrameData.batchCount);
        ImGui::Text("%d vertices", m_prevFrameData.vertexCount);
        ImGui::Text("%d indices", m_prevFrameData.indexCount);
        ImGui::End();
    }
}

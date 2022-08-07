//
// Created by aiden on 7/24/22.
//

#include "VkRenderer2DLowLevel.h"
#include "VkVertexBuffer.h"
#include "VkShader.h"

#include "incbin.h"

INCBIN(WhiteTexturePNG, "eugine/rendering/textures/WhiteTexture.png");

namespace eg::rendering::VKWrapper {
    Ref<VkVertexBuffer> VkRenderer2DLowLevel::createVertexBufferFN(void* userData) {
        auto* self = static_cast<VkRenderer2DLowLevel*>(userData);
        return createRef<VkVertexBuffer>(self->m_api.m_device, self->m_api.m_commandPool, self->m_api.m_allocator, nullptr,
                              self->m_settings.maxQuadsPerBatch * sizeof(rendering::Renderer2D::QuadVertex) * 4,
                              VertexBuffer::VB_USAGE_HINT_DYNAMIC);
    }

    Ref<VkIndexBuffer> VkRenderer2DLowLevel::createIndexBufferFN(void* userData) {
        auto* self = static_cast<VkRenderer2DLowLevel*>(userData);
        return createRef<VkIndexBuffer>(self->m_api.m_device, self->m_api.m_commandPool, self->m_api.m_allocator, nullptr,
                             self->m_settings.maxQuadsPerBatch * 6, VertexBuffer::VB_USAGE_HINT_DYNAMIC);
    }

    Ref<VkUniformBuffer> VkRenderer2DLowLevel::createUniformBufferFN(void* userData) {
        auto self = static_cast<VkRenderer2DLowLevel*>(userData);
        return createRef<VkUniformBuffer>(self->m_api.m_device, self->m_api.m_commandPool, self->m_api.m_allocator, nullptr,
                               sizeof(glm::mat4), VertexBuffer::VB_USAGE_HINT_DYNAMIC);
    }

    VkRenderer2DLowLevel::VkRenderer2DLowLevel(VKAPI& vkapi, rendering::Renderer2D::Settings settings) : m_api(vkapi),
                                                                                                         m_settings(
                                                                                                                 settings){
        trace("in VkRenderer2DLowLevel constructor: {}", settings.maxTextures);
        m_defaultTexture = vkapi.createTextureFromData(gWhiteTexturePNGData, gWhiteTexturePNGSize, "WhiteTexture.png");

        for(auto& combo : m_descriptorSetAllocators) {
            combo.textureArrayAllocator.init({0.0f, 128.0f, 5});
            combo.uniformBufferAllocator.init({1.0f, 0.0f, 5});
        }
    }


    void VkRenderer2DLowLevel::begin(Camera2D camera, Ref<Shader> shader) {
        auto vkShader = std::static_pointer_cast<VkShader>(shader);
        vkShader->setDescriptorAllocatorCombinations(m_descriptorSetAllocators.data());

        m_uniformBufferAllocators[m_api.getFrameInFlight()].resetAllocations();
        m_indexBufferAllocators[m_api.getFrameInFlight()].resetAllocations();
        m_vertexBufferAllocators[m_api.getFrameInFlight()].resetAllocations();

        m_descriptorSetAllocators[m_api.getFrameInFlight()].uniformBufferAllocator.resetAllocations();
        m_descriptorSetAllocators[m_api.getFrameInFlight()].textureArrayAllocator.resetAllocations();

        m_api.bindShader(vkShader);

        Ref<VkUniformBuffer> uniformBuffer = m_uniformBufferAllocators[m_api.getFrameInFlight()].allocate();
        glm::mat4 projxview = camera.getProjectionTimesView();
        uniformBuffer->setData(&projxview, sizeof(glm::mat4));
        vkShader->setUniformBuffer("PerFrameUBO", Ref<VkUniformBuffer>(uniformBuffer));
    }

    void
    VkRenderer2DLowLevel::drawCall(Renderer2D::QuadVertex* quadVertexData, Renderer2D::IndicesData* indicesData,
                                   u32 quadCount,
                                   Ref<rendering::Texture>* textures, u32 textureCount) {
        auto vertexBuffer = m_vertexBufferAllocators[m_api.getFrameInFlight()].allocate();
        auto indexBuffer  = m_indexBufferAllocators[m_api.getFrameInFlight()].allocate();

        vertexBuffer->setData((void*) quadVertexData, sizeof(Renderer2D::QuadVertex) * 4 * quadCount);
        indexBuffer->setData((const u16*) indicesData, sizeof(Renderer2D::IndicesData) * quadCount);

        m_api.m_currentBoundShader->setTextureArray("samplers", textures, textureCount);

        m_api.drawIndexed(vertexBuffer, indexBuffer);
    }

    void VkRenderer2DLowLevel::end() {
    }

    VkRenderer2DLowLevel::~VkRenderer2DLowLevel() {
        for(auto& combo : m_descriptorSetAllocators) {
            combo.textureArrayAllocator.destruct();
            combo.uniformBufferAllocator.destruct();
        }
    }

}
//
// Created by aiden on 7/24/22.
//

#ifndef EUGINE_VKRENDERER2DLOWLEVEL_H
#define EUGINE_VKRENDERER2DLOWLEVEL_H

#include "eugine/rendering/Renderer2DLowLevel.h"

#include "VKAPI.h"
#include "VkDynamicBufferFrameAllocator.h"
#include "VkDescriptorSetAllocator.h"

namespace eg::rendering::VKWrapper {
    class VkRenderer2DLowLevel : public Renderer2DLowLevel {
    public:
        VkRenderer2DLowLevel(VKAPI& vkapi, rendering::Renderer2D::Settings settings);

        ~VkRenderer2DLowLevel();

        void begin(Camera2D camera, Ref <Shader> shader) override;

        void drawCall(Renderer2D::QuadVertex* quadVertexData, Renderer2D::IndicesData* indicesData, u32 quadCount,
                      Ref <rendering::Texture>* textures, u32 textureCount) override;

        void end() override;

    private:
        VKAPI& m_api;
        rendering::Renderer2D::Settings m_settings;

        static Ref<VkVertexBuffer> createVertexBufferFN(void* userData);
        static Ref<VkIndexBuffer> createIndexBufferFN(void* userData);
        static Ref<VkUniformBuffer> createUniformBufferFN(void* userData);

        std::vector<VkDynamicBufferFrameAllocator<VkVertexBuffer>> m_vertexBufferAllocators{
                static_cast<u32>(m_api.maxFramesInFlight),
                VkDynamicBufferFrameAllocator<VkVertexBuffer>{createVertexBufferFN, this}};
        std::vector<VkDynamicBufferFrameAllocator<VkIndexBuffer>> m_indexBufferAllocators{
                static_cast<u32>(m_api.maxFramesInFlight),
                VkDynamicBufferFrameAllocator<VkIndexBuffer>{createIndexBufferFN, this}
        };
        std::vector<VkDynamicBufferFrameAllocator<VkUniformBuffer>> m_uniformBufferAllocators{
                static_cast<u32>(m_api.maxFramesInFlight),
                VkDynamicBufferFrameAllocator<VkUniformBuffer>{createUniformBufferFN, this}
        };

        Ref<VkTexture> m_defaultTexture = nullptr;

        std::array<VKAPI::DescriptorSetAllocatorCombination, VKAPI::maxFramesInFlight> m_descriptorSetAllocators = {
                VKAPI::DescriptorSetAllocatorCombination {
                        VkDescriptorSetAllocator(m_api.m_device),
                        VkDescriptorSetAllocator(m_api.m_device)
                },
                VKAPI::DescriptorSetAllocatorCombination {
                        VkDescriptorSetAllocator(m_api.m_device),
                        VkDescriptorSetAllocator(m_api.m_device)
                }
        };
    };
}


#endif //EUGINE_VKRENDERER2DLOWLEVEL_H

//
// Created by aiden on 6/15/22.
//

#ifndef EUGINE_VKSHADER_H
#define EUGINE_VKSHADER_H

#include <volk.h>

#include <eugine/rendering/Shader.h>
#include <eugine/rendering/VertexBufferLayout.h>
#include <shaderc/shaderc.hpp>

#include "VkTexture.h"
#include "VKAPI.h"

namespace eg::rendering::VKWrapper{
    class VkDevice;

    class VkRenderPass;

    class VkWindow;

    class VkUniformBuffer;

    class VkShader : public ::eg::rendering::Shader {
    public:
        VkShader(VkDevice& device, VkRenderPass& renderPass, VkWindow& window, VKAPI::DescriptorSetAllocatorCombination* descriptorSetAllocatorCombinations, u32 numFramesInFlight, const u32& currentFrameInFlight);

        ~VkShader();

        void init(eg::rendering::Shader::ShaderProgramSource source, eg::rendering::VertexBufferLayout& layout,
                  ShaderUniformLayout uniformLayout);
        void destruct();

        VkPipeline& getPipeline() { return m_pipeline; }
        VkPipelineLayout& getPipelineLayout() { return m_pipelineLayout; }

        u32 getPushConstantsSize() const { return m_pushConstantBufferSize; }
        u8* getPushConstantsBuffer() { return m_pushConstantBuffer; }

        void resetDescriptorBindState();

        void bind() const override;
        void unBind() const override;

        void setUniformBuffer(const char* name, Ref<UniformBuffer> uniformBuffer) override;
        void setUniformBuffer(const char* name, Ref<VkUniformBuffer> uniformBuffer);

        void setTextureArray(const char* name, Ref <Texture>* textures, u32 count) override;

        void setSamplerArray(const char* name, VkTexture** textures, u32 count);

        void setPushConstantUniform(const char* name, const void* data, u32 size);

        void setMat4(const char* name, const glm::mat4& mat) override;

        void setMat3(const char* name, const glm::mat3& mat) override;

        void setMat2(const char* name, const glm::mat2& mat) override;

        void setVec4(const char* name, const glm::vec4& vec) override;

        void setVec3(const char* name, const glm::vec3& vec) override;

        void setVec2(const char* name, const glm::vec2& vec) override;

        void setBool(const char* name, const bool& value) override;

        void setInt(const char* name, const i32& value) override;

        void setUint(const char* name, const u32& value) override;

        void setFloat(const char* name, const float& value) override;

        void setDouble(const char* name, const double& value) override;

        void setIntArray(const char* name, const i32* value, u32 count) override;


    private:
        friend class VKAPI;
        VkDevice& m_device;
        VkRenderPass& m_renderPass;
        VkWindow& m_window;

        VkPipeline m_pipeline = VK_NULL_HANDLE;
        VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;

        VkShaderModule createShaderModule(eg::rendering::Shader::ShaderUnitSource source, shaderc_shader_kind type,
                                          bool optimize = false);

        u32 m_pushConstantBufferSize = 0;
        u8* m_pushConstantBuffer = nullptr;
        std::unordered_map<std::string, u8*> m_pushConstantNamesToBufPtrMap;

        struct DescriptorSetInfo {
            u32 setNum = 0;
            VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
            bool needRebind = true;
        };

        std::unordered_map<std::string, DescriptorSetInfo> m_descriptorBindingNameToSetIndexMap;
        VkDescriptorSetLayout* m_descriptorSetLayouts = nullptr;
        u32 m_descriptorSetLayoutsCount = 0;
        VKAPI::DescriptorSetAllocatorCombination* m_descriptorSetAllocators;

        const u32 m_numFramesInFlight;
        const u32& m_currentFrameInFlight;
    };
}


#endif //EUGINE_VKSHADER_H

//
// Created by aiden on 6/15/22.
//

#ifndef EUGINE_VKSHADER_H
#define EUGINE_VKSHADER_H

#include <volk.h>

#include <eugine/rendering/Shader.h>
#include <eugine/rendering/VertexBufferLayout.h>
#include <shaderc/shaderc.hpp>

namespace eg::rendering::VKWrapper {
    class VkDevice;

    class VkRenderPass;

    class VkWindow;

    class VkShader {
    public:
        VkShader(VkDevice& device, VkRenderPass& renderPass, VkWindow& window);
        ~VkShader();

        void init(eg::rendering::Shader::ShaderProgramSource source, eg::rendering::VertexBufferLayout& layout);
        void destruct();

        VkPipeline& getPipeline() { return m_pipeline; }


    private:
        VkDevice& m_device;
        VkRenderPass& m_renderPass;
        VkWindow& m_window;

        VkPipeline m_pipeline = VK_NULL_HANDLE;
        VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;

        VkShaderModule createShaderModule(eg::rendering::Shader::ShaderUnitSource source, shaderc_shader_kind type, bool optimize = false);
    };
}


#endif //EUGINE_VKSHADER_H

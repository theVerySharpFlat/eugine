//
// Created by aiden on 6/15/22.
//

#include "VkShader.h"
#include "VkDevice.h"
#include "VkRenderPass.h"
#include "VkWindow.h"

#include <glm/gtc/type_ptr.hpp>

namespace eg::rendering::VKWrapper {
    VkShader::VkShader(VkDevice& device, VkRenderPass& renderPass, VkWindow& window) : m_device(device),
                                                                                       m_renderPass(renderPass),
                                                                                       m_window(window) {}

    VkShader::~VkShader() {
        destruct();
    }

    static VkFormat vertexAttributeToVkFormat(const VertexAttrib& attrib) {
        EG_ASSERT(attrib.count <= 4, "cannot have more than 4 primitives per attribute!!!");
        switch (attrib.type) {
            case SHDR_BOOL: EG_ASSERT(attrib.count == 1,
                                      "we do not support multiple booleans in vertex descriptions!!!");
                return VK_FORMAT_R8_UINT;
                break;
            case SHDR_INT:
                if (attrib.count == 1) {
                    return VK_FORMAT_R32_SINT;
                } else if (attrib.count == 2) {
                    return VK_FORMAT_R32G32_SINT;
                } else if (attrib.count == 3) {
                    return VK_FORMAT_R32G32B32_SINT;
                } else if (attrib.count == 4) {
                    return VK_FORMAT_R32G32B32A32_SINT;
                } else {
                    EG_ASSERT(false, "this should never happen");
                    return VK_FORMAT_R32_SINT;
                }
                break;
            case SHDR_UINT:
                if (attrib.count == 1) {
                    return VK_FORMAT_R32_UINT;
                } else if (attrib.count == 2) {
                    return VK_FORMAT_R32G32_UINT;
                } else if (attrib.count == 3) {
                    return VK_FORMAT_R32G32B32_UINT;
                } else if (attrib.count == 4) {
                    return VK_FORMAT_R32G32B32A32_UINT;
                } else {
                    EG_ASSERT(false, "this should never happen");
                    return VK_FORMAT_R32_SINT;
                }
                break;
            case SHDR_FLOAT:
                if (attrib.count == 1) {
                    return VK_FORMAT_R32_SFLOAT;
                } else if (attrib.count == 2) {
                    return VK_FORMAT_R32G32_SFLOAT;
                } else if (attrib.count == 3) {
                    return VK_FORMAT_R32G32B32_SFLOAT;
                } else if (attrib.count == 4) {
                    return VK_FORMAT_R32G32B32A32_SFLOAT;
                } else {
                    EG_ASSERT(false, "this should never happen");
                    return VK_FORMAT_R32_SINT;
                }
                break;
            case SHDR_DOUBLE:
                if (attrib.count == 1) {
                    return VK_FORMAT_R64_SFLOAT;
                } else if (attrib.count == 2) {
                    return VK_FORMAT_R64G64_SFLOAT;
                } else if (attrib.count == 3) {
                    return VK_FORMAT_R64G64B64_SFLOAT;
                } else if (attrib.count == 4) {
                    return VK_FORMAT_R64G64B64A64_SFLOAT;
                } else {
                    EG_ASSERT(false, "this should never happen");
                    return VK_FORMAT_R32_SINT;
                }
                break;
            case SHDR_VEC2: EG_ASSERT(attrib.count == 1, "you can only have 1 vec2!!!");
                return VK_FORMAT_R32G32_SFLOAT;
                break;
            case SHDR_VEC3: EG_ASSERT(attrib.count == 1, "you can only have 1 vec3!!!");
                return VK_FORMAT_R32G32B32_SFLOAT;
                break;
            case SHDR_VEC4: EG_ASSERT(attrib.count == 1, "you can only have 1 vec4!!!");
                return VK_FORMAT_R32G32B32A32_SFLOAT;
                break;
            case SHDR_MAT2: EG_ASSERT(false, "you can't have matrices in vertex input descriptions!!!");
                break;
            case SHDR_MAT3: EG_ASSERT(false, "you can't have matrices in vertex input descriptions!!!");
                break;
            case SHDR_MAT4: EG_ASSERT(false, "you can't have matrices in vertex input descriptions!!!");
                break;
        }
        EG_ASSERT(false, "failed to find vertex attribute type conversion to a vulkan type!!!");
        return VK_FORMAT_R32_SINT;
    }

    static VkVertexInputBindingDescription vertexBufferLayoutToInputBindingDescription(VertexBufferLayout& layout) {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        bindingDescription.stride = layout.getStride();
        return bindingDescription;
    }

    static void vertexBufferLayoutToAttributeDescriptions(VertexBufferLayout& layout,
                                                          VkVertexInputAttributeDescription* descriptions) {
        auto& attributes = layout.getAttributes();
        u32 byteOffset = 0;
        for (u32 i = 0; i < layout.getAttributes().size(); i++) {
            VkVertexInputAttributeDescription& attributeDescription = descriptions[i];
            attributeDescription.binding = 0;
            attributeDescription.location = i;
            attributeDescription.format = vertexAttributeToVkFormat(attributes[i]);
            attributeDescription.offset = byteOffset;

            byteOffset += getSizeOfType(attributes[i].type) * attributes[i].count;
        }

        trace("computed byte offset is {} bytes", byteOffset);
    }

    void VkShader::init(eg::rendering::Shader::ShaderProgramSource source, eg::rendering::VertexBufferLayout& layout,
                        ShaderUniformLayout uniformLayout) {
        VkShaderModule vertexShaderModule = createShaderModule(source.vs, shaderc_vertex_shader);
        VkShaderModule fragmentShaderModule = createShaderModule(source.fs, shaderc_fragment_shader);

        VkPipelineShaderStageCreateInfo vertexShaderStageCreateInfo{};
        vertexShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertexShaderStageCreateInfo.module = vertexShaderModule;
        vertexShaderStageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertexShaderStageCreateInfo.pName = "main";

        VkPipelineShaderStageCreateInfo fragmentShaderStageCreateInfo{};
        fragmentShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragmentShaderStageCreateInfo.module = fragmentShaderModule;
        fragmentShaderStageCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragmentShaderStageCreateInfo.pName = "main";

        VkPipelineShaderStageCreateInfo shaderStages[] = {
                vertexShaderStageCreateInfo,
                fragmentShaderStageCreateInfo
        };


        VkVertexInputBindingDescription bindingDescription = vertexBufferLayoutToInputBindingDescription(layout);
        auto attributeDescriptions = (VkVertexInputAttributeDescription*) alloca(
                sizeof(VkVertexInputAttributeDescription) * layout.getAttributes().size());
        vertexBufferLayoutToAttributeDescriptions(layout, attributeDescriptions);

        VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo{};
        vertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputStateCreateInfo.vertexBindingDescriptionCount = 1;
        vertexInputStateCreateInfo.pVertexBindingDescriptions = &bindingDescription;
        vertexInputStateCreateInfo.vertexAttributeDescriptionCount = layout.getAttributes().size();
        vertexInputStateCreateInfo.pVertexAttributeDescriptions = attributeDescriptions;

        VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo{};
        inputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float) m_window.getSwapchainExtent().width;
        viewport.height = (float) m_window.getSwapchainExtent().height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = m_window.getSwapchainExtent();

        VkPipelineViewportStateCreateInfo viewportStateCreateInfo{};
        viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportStateCreateInfo.scissorCount = 1;
        viewportStateCreateInfo.pScissors = &scissor;
        viewportStateCreateInfo.viewportCount = 1;
        viewportStateCreateInfo.pViewports = &viewport;

        VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo{};
        rasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizationStateCreateInfo.depthClampEnable = VK_FALSE;
        rasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
        rasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizationStateCreateInfo.lineWidth = 1.0f;
        rasterizationStateCreateInfo.cullMode = VK_CULL_MODE_NONE;
        rasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
        rasterizationStateCreateInfo.depthBiasEnable = VK_FALSE;
        rasterizationStateCreateInfo.depthBiasConstantFactor = 0.0f;
        rasterizationStateCreateInfo.depthBiasClamp = 0.0f;
        rasterizationStateCreateInfo.depthBiasSlopeFactor = 0.0f;

        VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo{};
        multisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
        multisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisampleStateCreateInfo.minSampleShading = 1.0f;
        multisampleStateCreateInfo.pSampleMask = nullptr;
        multisampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE;
        multisampleStateCreateInfo.alphaToOneEnable = VK_FALSE;

        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT
                                              | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_TRUE;
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
        colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

        VkPipelineColorBlendStateCreateInfo colorBlending{};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;

        m_pushConstantBufferSize = calculateShaderUniformLayoutSize(uniformLayout);
        m_pushConstantBuffer = (u8*) malloc(m_pushConstantBufferSize);

        if (m_pushConstantBufferSize > 128) {
            warn("push constant buffer has a size of {} which is greater than 128 bytes!!! Cannot guarantee that this shader will work!!!",
                 m_pushConstantBufferSize);
        }

        {
            u8* curPtr = m_pushConstantBuffer;
            for (const auto& u: uniformLayout.uniforms) {
                trace("uniform name: {}", u.name);
                m_pushConstantNamesToBufPtrMap[u.name] = curPtr;
                curPtr += getSizeOfType(u.type);
            }
        }

        VkPushConstantRange pushConstant{};
        pushConstant.offset = 0;
        pushConstant.size = m_pushConstantBufferSize;
        pushConstant.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

        m_descriptorSetLayouts = (VkDescriptorSetLayout*) malloc(
                sizeof(VkDescriptorSetLayout) * uniformLayout.bindings.size());
        m_descriptorSetLayoutsCount = uniformLayout.bindings.size();
        {
            u32 i = 0;
            for (auto& binding: uniformLayout.bindings) {
                m_descriptorBindingNameToSetIndexMap[binding.name] = i;

                VkDescriptorType descriptorType;
                switch (binding.type) {
                    case SHADER_BINDING_TYPE_SAMPLER_ARRAY:
                        descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                        break;
                    case SHADER_BINDING_TYPE_UNIFORM_BUFFER:
                        descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                        break;
                    default:
                        EG_ASSERT(false, "incorrect descriptor type. THIS SHOULD NEVER HAPPEN!!!");
                        break;
                }

                VkDescriptorSetLayoutBinding descriptorSetLayoutBinding{};
                descriptorSetLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
                descriptorSetLayoutBinding.binding = 0;
                descriptorSetLayoutBinding.descriptorCount = binding.arrayCount;
                descriptorSetLayoutBinding.descriptorType = descriptorType;
                descriptorSetLayoutBinding.pImmutableSamplers = nullptr;

                VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo{};
                descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
                descriptorSetLayoutCreateInfo.bindingCount = 1;
                descriptorSetLayoutCreateInfo.pBindings = &descriptorSetLayoutBinding;

                if(vkCreateDescriptorSetLayout(m_device.getDevice(), &descriptorSetLayoutCreateInfo, nullptr, m_descriptorSetLayouts + i) != VK_SUCCESS) {
                    error("failed to create descriptor set layout {} for shader \"{}\"", i, source.vs.name);
                }

                i++;
            }
        }

        VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
        pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutCreateInfo.pushConstantRangeCount = 1;
        pipelineLayoutCreateInfo.pPushConstantRanges = &pushConstant;
        pipelineLayoutCreateInfo.setLayoutCount = m_descriptorSetLayoutsCount;
        pipelineLayoutCreateInfo.pSetLayouts = m_descriptorSetLayouts;

        if (vkCreatePipelineLayout(m_device.getDevice(), &pipelineLayoutCreateInfo, nullptr, &m_pipelineLayout) !=
            VK_SUCCESS) {
            error("failed to create pipeline layout!");
            return;
        }

        VkDynamicState dynamicStates[] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};

        VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo{};
        dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicStateCreateInfo.pDynamicStates = dynamicStates;
        dynamicStateCreateInfo.dynamicStateCount = 2;

        VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo{};
        graphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        graphicsPipelineCreateInfo.stageCount = 2;
        graphicsPipelineCreateInfo.pStages = shaderStages;
        graphicsPipelineCreateInfo.pInputAssemblyState = &inputAssemblyStateCreateInfo;
        graphicsPipelineCreateInfo.pVertexInputState = &vertexInputStateCreateInfo;
        graphicsPipelineCreateInfo.pTessellationState = nullptr;
        graphicsPipelineCreateInfo.pViewportState = &viewportStateCreateInfo;
        graphicsPipelineCreateInfo.pRasterizationState = &rasterizationStateCreateInfo;
        graphicsPipelineCreateInfo.pMultisampleState = &multisampleStateCreateInfo;
        graphicsPipelineCreateInfo.pDepthStencilState = nullptr;
        graphicsPipelineCreateInfo.pColorBlendState = &colorBlending;
        graphicsPipelineCreateInfo.pDynamicState = &dynamicStateCreateInfo;
        graphicsPipelineCreateInfo.layout = m_pipelineLayout;
        graphicsPipelineCreateInfo.renderPass = m_renderPass.getRenderPass();
        graphicsPipelineCreateInfo.subpass = 0;
        graphicsPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
        graphicsPipelineCreateInfo.basePipelineIndex = -1;

        if (vkCreateGraphicsPipelines(m_device.getDevice(), VK_NULL_HANDLE, 1, &graphicsPipelineCreateInfo, nullptr,
                                      &m_pipeline) != VK_SUCCESS) {
            error("failed to create graphics pipeline!");
            return;
        }

        vkDestroyShaderModule(m_device.getDevice(), vertexShaderModule, nullptr);
        vkDestroyShaderModule(m_device.getDevice(), fragmentShaderModule, nullptr);

    }

    VkShaderModule
    VkShader::createShaderModule(eg::rendering::Shader::ShaderUnitSource source, shaderc_shader_kind type,
                                 bool optimize) {
        shaderc::Compiler compiler;
        shaderc::CompileOptions options;
        if (optimize)
            options.SetOptimizationLevel(shaderc_optimization_level_size);

        shaderc::SpvCompilationResult spirvResult = compiler.CompileGlslToSpv(source.data, source.size,
                                                                              type, source.name, "main",
                                                                              options);
        if (spirvResult.GetCompilationStatus() != shaderc_compilation_status_success) {
            error("failed to compile file \"{}\"", source.name);
            error("errors: {}", spirvResult.GetErrorMessage());
            return VK_NULL_HANDLE;
        }

        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.pCode = spirvResult.cbegin();
        createInfo.codeSize = sizeof(u32) * std::distance(spirvResult.cbegin(), spirvResult.cend());

        VkShaderModule module;
        if (vkCreateShaderModule(m_device.getDevice(), &createInfo, nullptr, &module) != VK_SUCCESS) {
            fatal("failed to create shader module!!!");
            return VK_NULL_HANDLE;
        }

        return module;
    }

    void VkShader::destruct() {
        for(u32 i = 0; i < m_descriptorSetLayoutsCount; i++) {
            vkDestroyDescriptorSetLayout(m_device.getDevice(), m_descriptorSetLayouts[i], nullptr);
        }

        if (m_pipelineLayout != VK_NULL_HANDLE)
            vkDestroyPipelineLayout(m_device.getDevice(), m_pipelineLayout, nullptr);
        if (m_pipeline != VK_NULL_HANDLE)
            vkDestroyPipeline(m_device.getDevice(), m_pipeline, nullptr);

        m_pipelineLayout = VK_NULL_HANDLE;
        m_pipeline = VK_NULL_HANDLE;

        free(m_pushConstantBuffer);
        free(m_descriptorSetLayouts);
    }

    void VkShader::bind() const {}

    void VkShader::unBind() const {}

    void VkShader::setPushConstantUniform(const char* name, const void* data, u32 size) {
        auto it = m_pushConstantNamesToBufPtrMap.find(name);

        if (it == m_pushConstantNamesToBufPtrMap.end()) {
            error("failed to find uniform \"{}\"", name);
            return;
        }

        memcpy(it->second, data, size);
    }

    void VkShader::setMat4(const char* name, const glm::mat4& mat) {
        setPushConstantUniform(name, glm::value_ptr(mat), getSizeOfType(SHDR_MAT4));
    }

    void VkShader::setMat3(const char* name, const glm::mat3& mat) {
        setPushConstantUniform(name, glm::value_ptr(mat), getSizeOfType(SHDR_MAT3));
    }

    void VkShader::setMat2(const char* name, const glm::mat2& mat) {
        setPushConstantUniform(name, glm::value_ptr(mat), getSizeOfType(SHDR_MAT2));
    }

    void VkShader::setVec4(const char* name, const glm::vec4& vec) {
        setPushConstantUniform(name, glm::value_ptr(vec), getSizeOfType(SHDR_VEC4));
    }

    void VkShader::setVec3(const char* name, const glm::vec3& vec) {
        setPushConstantUniform(name, glm::value_ptr(vec), getSizeOfType(SHDR_VEC3));
    }

    void VkShader::setVec2(const char* name, const glm::vec2& vec) {
        setPushConstantUniform(name, glm::value_ptr(vec), getSizeOfType(SHDR_VEC2));
    }

    void VkShader::setBool(const char* name, const bool& value) {
        setPushConstantUniform(name, &value, getSizeOfType(SHDR_BOOL));
    }

    void VkShader::setInt(const char* name, const i32& value) {
        setPushConstantUniform(name, &value, getSizeOfType(SHDR_INT));
    }

    void VkShader::setUint(const char* name, const u32& value) {
        setPushConstantUniform(name, &value, getSizeOfType(SHDR_UINT));
    }

    void VkShader::setFloat(const char* name, const float& value) {
        setPushConstantUniform(name, &value, getSizeOfType(SHDR_FLOAT));
    }

    void VkShader::setDouble(const char* name, const double& value) {
        setPushConstantUniform(name, &value, getSizeOfType(SHDR_DOUBLE));
    }

    void VkShader::setIntArray(const char* name, const i32* value, u32 count) {
        setPushConstantUniform(name, value, getSizeOfType(SHDR_INT) * count);
    }
}
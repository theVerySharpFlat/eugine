#include "Shader.h"
#include "UniformBuffer.h"
#include "error.h"
#include "glad/glad.h"

#include "eugine/core/log.h"

#include "glm/gtc/type_ptr.hpp"

namespace eg::rendering::GLWrapper {

    static void compileShader(u32& shader, GLenum type, ::eg::rendering::Shader::ShaderUnitSource source) {
        GLCall(shader = glCreateShader(type));
        GLCall(glShaderSource(shader, 1, &source.data, NULL));
        GLCall(glCompileShader(shader));

        i32 success;
        char infoLog[512];

        GLCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
        if (!success) {
            GLCall(glGetShaderInfoLog(shader, 512, NULL, infoLog));
            eg::error("SHADER COMPILATION ERROR (type = {0:x})", (int) type);
            eg::error("LOG: \n{}", infoLog);
        }

        EG_ASSERT(success, "failed to compile shader");
    }

    static void linkProgram(const u32& program) {
        GLCall(glLinkProgram(program));

        i32 success;
        char infoLog[512];

        GLCall(glGetProgramiv(program, GL_LINK_STATUS, &success))
        if (!success) {
            GLCall(glGetProgramInfoLog(program, 512, NULL, infoLog));
            eg::error("SHADER LINKING FAILED: ");
            eg::error("LOG: \n{}", infoLog);
        }

        EG_ASSERT(success, "failed to compile shader");
    }

    Shader::Shader(const ShaderProgramSource& source, ShaderUniformLayout layout,
                   VertexBufferLayout vertexBufferLayout) {
        GLCall(m_ID = glCreateProgram());

        u32 vertexShader;
        u32 fragmentShader;

        compileShader(vertexShader, GL_VERTEX_SHADER, source.vs);
        compileShader(fragmentShader, GL_FRAGMENT_SHADER, source.fs);

        GLCall(glAttachShader(m_ID, vertexShader));
        GLCall(glAttachShader(m_ID, fragmentShader));
        linkProgram(m_ID);

        GLCall(glDeleteShader(vertexShader));
        GLCall(glDeleteShader(fragmentShader));

        for (auto& uniformDescription: layout.uniforms) {
            m_uniformNameToShaderLocationMap[std::string(uniformDescription.name)] = {
                    glGetUniformLocation(m_ID, uniformDescription.name), uniformDescription.type};
            GLCall("see line above";);
        }

        u32 uboBindingIndex = 0;
        for (auto& binding: layout.bindings) {
            m_bindingNameToBindingInfoMap[std::string(binding.name)] = {
                    binding.type,
                    binding.arrayCount,
                    UINT32_MAX,
                    uboBindingIndex
            };

            if (binding.type == SHADER_BINDING_TYPE_UNIFORM_BUFFER)
                uboBindingIndex++;
        }
    }

    void Shader::bind() const {
        GLCall(glUseProgram(m_ID));
    }

    void Shader::unBind() const {
        GLCall(glUseProgram(0));
    }

#define uniformVerficationFN(name, tp) \
    auto found = m_uniformNameToShaderLocationMap.find(name); \
    if(found == m_uniformNameToShaderLocationMap.end()) {      \
        error("could not find uniform \"{}\", name");\
        return;\
    } else if(found->second.type != (tp)) {\
        error("Type mismatch. Uniform \"{}\" is not of type {}", name, tp); \
        return;                                 \
    }

    void Shader::setMat4(const char* name, const glm::mat4& mat) {
        uniformVerficationFN(name, SHDR_MAT4);
        GLCall(glUseProgram(m_ID));
        GLCall(glUniformMatrix4fv(
                found->second.location,
                1,
                GL_FALSE,
                glm::value_ptr(mat)
        ));
    }

    void Shader::setMat3(const char* name, const glm::mat3& mat) {
        uniformVerficationFN(name, SHDR_MAT3)
        GLCall(glUseProgram(m_ID));
        GLCall(glUniformMatrix3fv(
                found->second.location,
                1,
                GL_FALSE,
                glm::value_ptr(mat)
        ));
    }

    void Shader::setMat2(const char* name, const glm::mat2& mat) {
        uniformVerficationFN(name, SHDR_MAT2)
        GLCall(glUseProgram(m_ID));
        GLCall(glUniformMatrix2fv(
                found->second.location,
                1,
                GL_FALSE,
                glm::value_ptr(mat)
        ));
    }

    void Shader::setVec4(const char* name, const glm::vec4& vec) {
        uniformVerficationFN(name, SHDR_VEC4)
        GLCall(glUseProgram(m_ID));
        GLCall(glUniform4fv(
                found->second.location,
                1,
                glm::value_ptr(vec)
        ));
    }

    void Shader::setVec3(const char* name, const glm::vec3& vec) {
        uniformVerficationFN(name, SHDR_VEC3)
        GLCall(glUseProgram(m_ID));
        GLCall(glUniform3fv(
                found->second.location,
                1,
                glm::value_ptr(vec)
        ));
    }

    void Shader::setVec2(const char* name, const glm::vec2& vec) {
        uniformVerficationFN(name, SHDR_VEC2)
        GLCall(glUseProgram(m_ID));
        GLCall(glUniform2fv(
                found->second.location,
                1,
                glm::value_ptr(vec)
        ));
    }

    void Shader::setBool(const char* name, const bool& value) {
        uniformVerficationFN(name, SHDR_BOOL)
        GLCall(glUseProgram(m_ID));
        GLCall(glUniform1i(
                found->second.location,
                (i32) value
        ));
    }

    void Shader::setInt(const char* name, const i32& value) {
        uniformVerficationFN(name, SHDR_INT)
        GLCall(glUseProgram(m_ID));
        GLCall(glUniform1i(
                found->second.location,
                (i32) value
        ));
    }

    void Shader::setUint(const char* name, const u32& value) {
        uniformVerficationFN(name, SHDR_UINT)
        GLCall(glUseProgram(m_ID));
        GLCall(glUniform1ui(
                found->second.location,
                value
        ));
    }

    void Shader::setFloat(const char* name, const float& value) {
        uniformVerficationFN(name, SHDR_FLOAT)
        GLCall(glUseProgram(m_ID));
        GLCall(glUniform1f(
                found->second.location,
                value
        ));
    }

    void Shader::setDouble(const char* name, const double& value) {
        uniformVerficationFN(name, SHDR_DOUBLE)
        GLCall(glUseProgram(m_ID));
        GLCall(glUniform1d(
                found->second.location,
                value
        ));
    }

    void Shader::setIntArray(const char* name, const i32* value, u32 count) {
        uniformVerficationFN(name, SHDR_INT)
        GLCall(glUseProgram(m_ID));
        GLCall(glUniform1iv(
                found->second.location,
                count,
                value
        ));
    }

    void Shader::setUniformBuffer(const char* name, Ref <rendering::UniformBuffer> uniformBuffer) {
        bind();
        auto lowLevelUBO = std::static_pointer_cast<GLWrapper::UniformBuffer>(uniformBuffer);
        lowLevelUBO->bind();

        auto found = m_bindingNameToBindingInfoMap.find(name);
        if (found->second.type != SHADER_BINDING_TYPE_UNIFORM_BUFFER) {
            error("Binding \"{}\" is not a uniform buffer binding!", name);
            return;
        } else if (found == m_bindingNameToBindingInfoMap.end()) {
            error("Binding \"{}\" does not exist!!!");
            return;
        }

        if (found->second.blockIndex == UINT32_MAX) {
            GLCall(found->second.blockIndex = glGetUniformBlockIndex(m_ID, name));
        }
        //trace("{} has a binding index of {}", name, found->second.bindingIndex);
        GLCall(glUniformBlockBinding(m_ID, found->second.blockIndex, found->second.bindingIndex));

        GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, found->second.bindingIndex, lowLevelUBO->m_ID));
    }

    void Shader::setTextureArray(const char* name, Ref <rendering::Texture>* textures, u32 count) {
        auto found = m_bindingNameToBindingInfoMap.find(name);
        if (found == m_bindingNameToBindingInfoMap.end()) {
            error("\"{}\" is not a binding!!!", name);
            return;
        } else if (found->second.type != SHADER_BINDING_TYPE_SAMPLER_ARRAY) {
            error("binding \"{}\" is not of type SHADER_BINDING_TYPE_SAMPLER_ARRAY", name);
            return;
        }

        auto samplerIndices = (i32*) alloca(sizeof(i32) * found->second.count);
        for (i32 i = 0; i < found->second.count; i++) {
            samplerIndices[i] = i;
        }

        GLCall(glUniform1iv(
                glGetUniformLocation(m_ID, name),
                found->second.count,
                samplerIndices
               ));

        for(u32 i = 0; i < count; i++) {
            auto texture = std::dynamic_pointer_cast<GLWrapper::Texture>(textures[i]);
            // trace("binding texture \"{}\" to slot {}", texture->getName(), i);
            texture->bind(i);
        }
    }

    Shader::~Shader() {
        GLCall(glDeleteProgram(m_ID));
    }

} // namespace eg::rendering::GLWrapper

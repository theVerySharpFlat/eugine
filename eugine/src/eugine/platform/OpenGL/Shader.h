#ifndef EG_PLATFORM_OPENGL_WRAPPER_SHADER_H
#define EG_PLATFORM_OPENGL_WRAPPER_SHADER_H

#include "eugine/core/core.h"
#include "eugine/rendering/Shader.h"
#include "glm/ext/matrix_float4x4.hpp"
#include "Texture.h"

namespace eg::rendering::GLWrapper {

    class EG_API Shader : public eg::rendering::Shader {
    public:
        Shader(const ShaderProgramSource& src, ShaderUniformLayout layout, VertexBufferLayout vertexBufferLayout);

        ~Shader();

        void bind() const;
        void unBind() const;



        void setTextureArray(const char* name, Ref<rendering::Texture>* textures, u32 count) override;

        void setUniformBuffer(const char* name, Ref<rendering::UniformBuffer> uniformBuffer) override;

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

        u32 m_ID;

        struct UniformLocation {
            i32 location;
            ShaderType type;
        };
        std::unordered_map<std::string, UniformLocation> m_uniformNameToShaderLocationMap;
        struct BindingInfo {
            ShaderBindingType type;
            u32 count;
            u32 blockIndex = UINT32_MAX;
            u32 bindingIndex;
        };
        std::unordered_map<std::string, BindingInfo> m_bindingNameToBindingInfoMap;
    };
} // namespace eg::rendering::OpenGLAPI

#endif

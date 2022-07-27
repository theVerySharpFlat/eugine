#ifndef EG_RENDERING_SHADER
#define EG_RENDERING_SHADER

#include <glm/glm.hpp>

#include "Types.h"
#include "VertexBufferLayout.h"
#include "UniformBuffer.h"
#include "Texture.h"

namespace eg::rendering {

    struct ShaderUniform {
        const char* name;
        ShaderType type;
    };

    enum ShaderBindingType {
        SHADER_BINDING_TYPE_SAMPLER_ARRAY = 0,
        SHADER_BINDING_TYPE_UNIFORM_BUFFER
    };

    struct ShaderBindingDescription {
        const char* name;
        ShaderBindingType type;
        u32 arrayCount;
    };

    struct ShaderUniformLayout {
        std::initializer_list<ShaderUniform> uniforms;
        std::initializer_list<ShaderBindingDescription> bindings;
    };

    u32 calculateShaderUniformLayoutSize(const ShaderUniformLayout& layout);

    class Shader {
    public:
        struct ShaderUnitSource {
            const char* name;
            const char *data;
            u64 size;
        };

        struct ShaderProgramSource {
            const ShaderUnitSource vs;
            const ShaderUnitSource fs;
        };

        static Ref<Shader> create(const ShaderProgramSource& source, ShaderUniformLayout layout, VertexBufferLayout vertexBufferLayout);

        virtual void bind() const = 0;
        virtual void unBind() const = 0;

        virtual void setTextureArray(const char* name, Ref<Texture>* textures, u32 count) = 0;
        virtual void setUniformBuffer(const char* name, Ref<UniformBuffer> uniformBuffer) = 0;

        virtual void setMat4(const char* name, const glm::mat4& mat) = 0;
        virtual void setMat3(const char* name, const glm::mat3& mat) = 0;
        virtual void setMat2(const char* name, const glm::mat2& mat) = 0;

        virtual void setVec4(const char* name, const glm::vec4& vec) = 0;
        virtual void setVec3(const char* name, const glm::vec3& vec) = 0;
        virtual void setVec2(const char* name, const glm::vec2& vec) = 0;

        virtual void setBool(const char* name, const bool& value) = 0;
        virtual void setInt(const char* name, const i32& value) = 0;
        virtual void setUint(const char* name, const u32& value) = 0;
        virtual void setFloat(const char* name, const float& value) = 0;
        virtual void setDouble(const char* name, const double& value) = 0;

        virtual void setIntArray(const char* name, const i32* value, u32 count) = 0;
    };
}

#endif
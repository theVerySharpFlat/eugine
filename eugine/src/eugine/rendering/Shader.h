#ifndef EG_RENDERING_SHADER
#define EG_RENDERING_SHADER

#include <glm/glm.hpp>

namespace eg::rendering {
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

        static Ref<Shader> create(const ShaderProgramSource& source);

        virtual void bind() const = 0;
        virtual void unBind() const = 0;

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
#ifndef EG_PLATFORM_OPENGL_WRAPPER_SHADER_H
#define EG_PLATFORM_OPENGL_WRAPPER_SHADER_H

#include <eugine/core/core.h>
#include <eugine/rendering/Shader.h>
#include "glm/ext/matrix_float4x4.hpp"

namespace eg {
    namespace GLWrapper {

        class EG_API Shader : public eg::rendering::Shader {
        public:
            Shader(const ShaderProgramSource& source);
            ~Shader();

            void bind() const override;
            void unBind() const override;

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

        private:

            u32 m_ID;
        };
    } // namespace glwrapper
} // namespace eg

#endif

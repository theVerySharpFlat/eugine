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

            void bind() const;
            void unBind() const;

            void setMat4(const char* name, const glm::mat4& mat);
            void setMat3(const char* name, const glm::mat3& mat);
            void setMat2(const char* name, const glm::mat2& mat);

            void setVec4(const char* name, const glm::vec4& vec);
            void setVec3(const char* name, const glm::vec3& vec);
            void setVec2(const char* name, const glm::vec2& vec);

            void setBool(const char* name, const bool& value);
            void setInt(const char* name, const i32& value);
            void setUint(const char* name, const u32& value);
            void setFloat(const char* name, const float& value);
            void setDouble(const char* name, const double& value);

        private:

            u32 m_ID;
        };
    } // namespace glwrapper
} // namespace eg

#endif

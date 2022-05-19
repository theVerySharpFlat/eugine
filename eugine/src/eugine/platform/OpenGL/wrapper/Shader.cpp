#include "Shader.h"
#include "error.h"
#include <glad/glad.h>

#include <eugine/core/log.h>

#include <glm/gtc/type_ptr.hpp>

namespace eg {
    namespace GLWrapper {

        static void compileShader(u32 &shader, GLenum type, ::eg::rendering::Shader::ShaderUnitSource source) {
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

        static void linkProgram(const u32 &program) {
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

        Shader::Shader(const ShaderProgramSource& source) {
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
        }

        void Shader::bind() const {
            GLCall(glUseProgram(m_ID));
        }

        void Shader::unBind() const {
            GLCall(glUseProgram(0));
        }

        void Shader::setMat4(const char *name, const glm::mat4& mat) {
            GLCall(glUseProgram(m_ID));
            GLCall(glUniformMatrix4fv(
                    glGetUniformLocation(m_ID, name),
                    1,
                    GL_FALSE,
                    glm::value_ptr(mat)
                    ));
        }

        void Shader::setMat3(const char *name, const glm::mat3& mat) {
            GLCall(glUseProgram(m_ID));
            GLCall(glUniformMatrix3fv(
                    glGetUniformLocation(m_ID, name),
                    1,
                    GL_FALSE,
                    glm::value_ptr(mat)
            ));
        }

        void Shader::setMat2(const char *name, const glm::mat2& mat) {
            GLCall(glUseProgram(m_ID));
            GLCall(glUniformMatrix2fv(
                    glGetUniformLocation(m_ID, name),
                    1,
                    GL_FALSE,
                    glm::value_ptr(mat)
            ));
        }

        void Shader::setVec4(const char *name, const glm::vec4& vec) {
            GLCall(glUseProgram(m_ID));
            GLCall(glUniform4fv(
                    glGetUniformLocation(m_ID, name),
                    1,
                    glm::value_ptr(vec)
                    ));
        }

        void Shader::setVec3(const char *name, const glm::vec3& vec) {
            GLCall(glUseProgram(m_ID));
            GLCall(glUniform3fv(
                    glGetUniformLocation(m_ID, name),
                    1,
                    glm::value_ptr(vec)
            ));
        }

        void Shader::setVec2(const char *name, const glm::vec2& vec) {
            GLCall(glUseProgram(m_ID));
            GLCall(glUniform2fv(
                    glGetUniformLocation(m_ID, name),
                    1,
                    glm::value_ptr(vec)
            ));
        }

        void Shader::setBool(const char *name, const bool& value) {
            GLCall(glUseProgram(m_ID));
            GLCall(glUniform1i(
                    glGetUniformLocation(m_ID, name),
                    (i32)value
                    ));
        }

        void Shader::setInt(const char *name, const i32& value) {
            GLCall(glUseProgram(m_ID));
            GLCall(glUniform1i(
                    glGetUniformLocation(m_ID, name),
                    (i32)value
            ));
        }

        void Shader::setUint(const char *name, const u32& value) {
            GLCall(glUseProgram(m_ID));
            GLCall(glUniform1ui(
                    glGetUniformLocation(m_ID, name),
                    value
                    ));
        }

        void Shader::setFloat(const char *name, const float& value) {
            GLCall(glUseProgram(m_ID));
            GLCall(glUniform1f(
                    glGetUniformLocation(m_ID, name),
                    value
                    ));
        }

        void Shader::setDouble(const char *name, const double& value) {
            GLCall(glUseProgram(m_ID));
            GLCall(glUniform1d(
                    glGetUniformLocation(m_ID, name),
                    value
                    ));
        }

        void Shader::setIntArray(const char *name, const i32 *value, u32 count) {
            GLCall(glUseProgram(m_ID));
            GLCall(glUniform1iv(
                    glGetUniformLocation(m_ID, name),
                    count,
                    value
                    ));
        }

        Shader::~Shader() {
            GLCall(glDeleteProgram(m_ID));
        }

    }
} // namespace eg

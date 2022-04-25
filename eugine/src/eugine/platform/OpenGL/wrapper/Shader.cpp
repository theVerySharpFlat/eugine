#include "Shader.h"
#include "error.h"
#include <glad/glad.h>

#include <eugine/core/log.h>

namespace eg {
    namespace GLWrapper {

        static void compileShader(u32& shader, GLenum type, ShaderUnitSource source) {
            GLCall(shader = glCreateShader(type));
            GLCall(glShaderSource(shader, 1, &source.data, NULL));
            GLCall(glCompileShader(shader));

            i32 success;
            char infoLog[512];

            GLCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
            if(!success) {
                GLCall(glGetShaderInfoLog(shader, 512, NULL, infoLog));
                eg::error("SHADER COMPILATION ERROR (type = {0:x})", (int)type);
                eg::error("LOG: \n{}", infoLog);
            }

            EG_ASSERT(success, "failed to compile shader");
        }

        static void linkProgram(const u32& program) {
            GLCall(glLinkProgram(program));

            i32 success;
            char infoLog[512];

            GLCall(glGetProgramiv(program, GL_LINK_STATUS, &success))
            if(!success) {
                GLCall(glGetProgramInfoLog(program, 512, NULL, infoLog));
                eg::error("SHADER LINKING FAILED: ");
                eg::error("LOG: \n{}", infoLog);
            }

            EG_ASSERT(success, "failed to compile shader");
        }

        Shader::Shader(const ShaderProgramSource &source) {
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

        void Shader::bind() {
            GLCall(glUseProgram(m_ID));
        }

        void Shader::unBind() {
            GLCall(glUseProgram(0));
        }

        Shader::~Shader() {
           GLCall(glDeleteProgram(m_ID));
        }
    }
} // namespace eg

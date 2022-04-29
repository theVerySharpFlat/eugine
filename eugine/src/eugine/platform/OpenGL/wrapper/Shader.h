#ifndef EG_PLATFORM_OPENGL_WRAPPER_SHADER_H
#define EG_PLATFORM_OPENGL_WRAPPER_SHADER_H

#include <eugine/core/core.h>

namespace eg {
    namespace GLWrapper {

        struct ShaderUnitSource {
            const char *data;
            u64 size;
        };

        struct ShaderProgramSource {
            const ShaderUnitSource vs;
            const ShaderUnitSource fs;
        };

        class EG_API Shader {
        public:
            Shader(const ShaderProgramSource source);

            ~Shader();

            void bind() const;

            void unBind() const;

        private:

            u32 m_ID;
        };
    } // namespace glwrapper
} // namespace eg

#endif

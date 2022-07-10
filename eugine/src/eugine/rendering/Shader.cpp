#include "Shader.h"
#include "eugine/platform/OpenGL/wrapper/Shader.h"
#include "eugine/rendering/Shader.h"

namespace eg::rendering {
    Ref<Shader> Shader::create(const ShaderProgramSource& src) {
        return createRef<GLWrapper::Shader>(src);
    }

    u32 calculateShaderUniformLayoutSize(const ShaderUniformLayout& layout) {
        u32 size = 0;
        for(const auto& u : layout.uniforms) {
            size += getSizeOfType(u.type);
        }
        return size;
    }
}
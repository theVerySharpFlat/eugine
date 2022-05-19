#include "Shader.h"
#include "eugine/platform/OpenGL/wrapper/Shader.h"
#include "eugine/rendering/Shader.h"

namespace eg::rendering {
    Ref<Shader> Shader::create(const ShaderProgramSource& src) {
        return createRef<GLWrapper::Shader>(src);
    }
}
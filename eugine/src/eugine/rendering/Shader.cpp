#include "Shader.h"
#include "eugine/platform/OpenGL/wrapper/Shader.h"
#include "eugine/platform/Vulkan/VKAPI.h"
#include "eugine/platform/Vulkan/VkShader.h"
#include "GraphicsAPI.h"

namespace eg::rendering {
    Ref<Shader> Shader::create(const ShaderProgramSource& src, ShaderUniformLayout layout, VertexBufferLayout vertexBufferLayout) {
        if(rendering::getPreferredGraphicsAPI() == EG_API_OGL)
            return createRef<GLWrapper::Shader>(src);
        else if(rendering::getPreferredGraphicsAPI() == EG_API_VK)
            return std::static_pointer_cast<Shader>(VKWrapper::VKAPI::get()->createShader(src, vertexBufferLayout, layout));
        EG_ASSERT(false, "invalid graphics API. This should **NEVER** happen!!!");
        return nullptr;
    }

    u32 calculateShaderUniformLayoutSize(const ShaderUniformLayout& layout) {
        u32 size = 0;
        for(const auto& u : layout.uniforms) {
            size += getSizeOfType(u.type);
        }
        return size;
    }
}
#ifndef EG_PLATFORM_OPENGL_WRAPPER_GL_API_H
#define EG_PLATFORM_OPENGL_WRAPPER_GL_API_H

#include "eugine/rendering/GraphicsAPI.h"
#include "eugine/core/window.h"

namespace eg::GLWrapper {
    class OGLAPI : public ::eg::rendering::GraphicsAPI {
    public:
        OGLAPI(Window& window); 
        ~OGLAPI() = default;

        void setClearColor(glm::vec3 color) override;
        void clear() override;

        void begin() override {}
        void end() override {}

        void deviceWaitIdle() override {}

        void swapBuffers() override;

        i32 getMaxTexturesPerShader() const override;
    
    private:
        Window& m_window;
    };
}

#endif
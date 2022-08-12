#ifndef EG_PLATFORM_OPENGL_WRAPPER_GL_API_H
#define EG_PLATFORM_OPENGL_WRAPPER_GL_API_H

#include "eugine/rendering/GraphicsAPI.h"
#include "eugine/core/window.h"

namespace eg::rendering::GLWrapper {
    class OpenGLAPI : public ::eg::rendering::GraphicsAPI {
    public:
        OpenGLAPI(Window& window);
        ~OpenGLAPI() = default;

        void setClearColor(glm::vec3 color) override;
        void clear() override;

        void begin() override { clear(); }
        void end() override {}

        void deviceWaitIdle() override {}

        void swapBuffers() override;

        i32 getMaxTexturesPerShader() const override;

        void imguiInit() override;

        void imguiShutdown() override;

        void imguiBegin() override;

        void imguiEnd() override;

    private:
        Window& m_window;
    };
}

#endif
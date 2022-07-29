#ifndef EG_RENDERING_GRAPHICS_API_H
#define EG_RENDERING_GRAPHICS_API_H

#include <eugine/core/window.h>
#include <glm/glm.hpp>

namespace eg::rendering {

    enum GraphicsAPIID {
        EG_API_OGL = 0,
        EG_API_VK
    };
    GraphicsAPIID getPreferredGraphicsAPI();

    class GraphicsAPI {
    public:
        static GraphicsAPI* create(Window& window);

        virtual ~GraphicsAPI(){};

        virtual void setClearColor(glm::vec3 color) = 0;
        virtual void clear() = 0;

        virtual void begin() = 0;
        virtual void end() = 0;

        virtual void deviceWaitIdle() = 0;

        virtual void swapBuffers() = 0;

        virtual i32 getMaxTexturesPerShader() const = 0;
    };
}

#endif
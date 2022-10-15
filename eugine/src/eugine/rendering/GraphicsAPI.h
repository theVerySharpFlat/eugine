#ifndef EG_RENDERING_GRAPHICS_API_H
#define EG_RENDERING_GRAPHICS_API_H

#include <eugine/core/window.h>
#include <eugine/rendering/Framebuffer.h>
#include <glm/glm.hpp>

namespace eg::rendering {

    enum GraphicsAPIID {
        EG_API_OGL = 0,
        EG_API_VK
    };
    GraphicsAPIID getPreferredGraphicsAPI();
    /**
     * Set the preferred graphics API
     * WARNING: this just sets a variable and doesn't recreate
     * objects created using the previous API
     * @param api
     */
    void setPreferredGraphicsAPI(GraphicsAPIID api);

    class GraphicsAPI {
    public:
        static GraphicsAPI* create(Window& window);
        static bool vulkanInitializationSuccessful();

        virtual ~GraphicsAPI(){};

        virtual void setClearColor(glm::vec3 color) = 0;
        virtual void clear() = 0;

        virtual void begin() = 0;
        virtual void end() = 0;

        virtual Framebuffer& getDefaultFramebuffer() {};
        virtual void beginRenderTarget(Framebuffer& framebuffer) {}//= 0;
        virtual void endRenderTarget() {}//= 0;

        virtual void deviceWaitIdle() = 0;

        virtual void swapBuffers() = 0;

        virtual i32 getMaxTexturesPerShader() const = 0;

        virtual void imguiInit() = 0;
        virtual void imguiShutdown() = 0;
        virtual void imguiBegin() = 0;
        virtual void imguiEnd() = 0;
        virtual void drawFramebuffer(Ref<rendering::Framebuffer> fb){}
    };
}

#endif
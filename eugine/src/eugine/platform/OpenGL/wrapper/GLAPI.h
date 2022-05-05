#ifndef EG_PLATFORM_OPENGL_WRAPPER_GL_API_H
#define EG_PLATFORM_OPENGL_WRAPPER_GL_API_H

#include "eugine/rendering/GraphicsAPI.h"

namespace eg::GLWrapper {
    class OGLAPI : public ::eg::rendering::GraphicsAPI {
    public:
        OGLAPI() {}
        ~OGLAPI() = default;

        void setClearColor(glm::vec3 color) override;
        void clear() override;
    };
}

#endif
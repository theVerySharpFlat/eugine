#ifndef EG_RENDERING_GRAPHICS_API_H
#define EG_RENDERING_GRAPHICS_API_H

#include <glm/glm.hpp>

namespace eg::rendering {
    class GraphicsAPI {
    public:
        static Ref<GraphicsAPI> create();

        virtual void setClearColor(glm::vec3 color) = 0;
        virtual void clear() = 0;
    };
}

#endif
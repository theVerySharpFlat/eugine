#include "eugine/platform/OpenGL/wrapper/GLAPI.h"
#include "error.h"
#include <glad/glad.h>

namespace eg::GLWrapper {
    void OGLAPI::setClearColor(glm::vec3 color) {
       GLCall(glClearColor(color.x, color.y, color.z, 1.0));
    }

    void OGLAPI::clear() {
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
    }
}
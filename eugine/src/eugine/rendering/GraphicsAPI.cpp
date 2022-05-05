#include "eugine/platform/OpenGL/wrapper/GLAPI.h"
#include "eugine/rendering/GraphicsAPI.h"

namespace eg::rendering {
    Ref<GraphicsAPI> GraphicsAPI::create() {
        return createRef<GLWrapper::OGLAPI>();
    }
}
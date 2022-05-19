#include "Texture.h"
#include "eugine/platform/OpenGL/wrapper/Texture.h"

namespace eg::rendering {
    Ref<Texture> Texture::create(const fs::path& path) {
        return createRef<GLWrapper::Texture>(path);
    }
}
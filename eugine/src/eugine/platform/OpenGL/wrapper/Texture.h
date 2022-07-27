#ifndef EG_PLATFORM_OPENGL_TEXTURE
#define EG_PLATFORM_OPENGL_TEXTURE

#include "eugine/rendering/Texture.h"

namespace eg::GLWrapper {
    class Texture : public ::eg::rendering::Texture{
    public:
        Texture(const fs::path& path);
        ~Texture();
        
        void bind(u32 slot) const;
        void unBind() const;

        bool operator==(const ::eg::rendering::Texture& texture) const override; 

    private:
        u32 m_ID = 0;
    };
}

#endif
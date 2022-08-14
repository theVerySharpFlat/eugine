#ifndef EG_PLATFORM_OPENGL_TEXTURE
#define EG_PLATFORM_OPENGL_TEXTURE

#include "eugine/rendering/Texture.h"

namespace eg::rendering::GLWrapper {
    class Texture : public ::eg::rendering::Texture{
    public:
        Texture(const fs::path& path);
        Texture(const u8* data, u32 size, const char* name);
        ~Texture();
        
        void bind(u32 slot) const;
        void unBind() const;

        bool operator==(const ::eg::rendering::Texture& texture) const override;

        const std::string& getName() { return m_name; }

    private:
        u32 m_ID = 0;

        std::string m_name;
    };
}

#endif
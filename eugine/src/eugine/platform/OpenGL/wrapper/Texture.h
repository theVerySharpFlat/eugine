#ifndef EG_PLATFORM_OPENGL_TEXTURE
#define EG_PLATFORM_OPENGL_TEXTURE

namespace eg::GLWrapper {
    class Texture {
    public:
        Texture(const fs::path& path);
        ~Texture();
        
        void bind() const;
        void unBind() const;

    private:
        u32 m_ID = 0;
    };
}

#endif
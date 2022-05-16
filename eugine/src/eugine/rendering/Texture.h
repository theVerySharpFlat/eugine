#ifndef EG_RENDERING_TEXTURE_H
#define EG_RENDERING_TEXTURE_H

namespace eg::rendering {
    class Texture {
    public:
        static Ref<Texture> create(const fs::path& path);

        virtual void bind(u32 slot) const = 0;
        virtual void unBind() const = 0;
    };
}

#endif
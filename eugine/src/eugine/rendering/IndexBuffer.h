#ifndef EG_RENDERING_INDEX_BUFFER_H
#define EG_RENDERING_INDEX_BUFFER_H

namespace eg::rendering {
    class IndexBuffer {
    public:
        static Ref<IndexBuffer> create(u32* data, u32 size);

        virtual void bind() const = 0;
        virtual void unBind() const = 0;

        virtual void setData(u32* data, u32 size) = 0;

        virtual u32 getElementCount() const = 0;
        virtual void setElementCount(u32 count) = 0;
    };
}

#endif
#ifndef EG_RENDERING_VERTEX_ARRAY
#define  EG_RENDERING_VERTEX_ARRAY

#include <eugine/rendering/VertexBuffer.h>

namespace eg::rendering {
    class VertexArray {
    public:
        static Ref<VertexArray> create();

        virtual void bind() const = 0;
        virtual void unBind() const = 0;

        virtual void setBuffer(const VertexBuffer& buffer) = 0;
    };
}

#endif
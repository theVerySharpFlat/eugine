#ifndef EG_RENDERING_LOW_LEVEL_RENDERER_H
#define EG_RENDERING_LOW_LEVEL_RENDERER_H

#include "eugine/rendering/VertexArray.h"
#include "eugine/rendering/Shader.h"
#include "eugine/rendering/IndexBuffer.h"

namespace eg::rendering {
    class LowLevelRenderer {
    public:
        static Ref<LowLevelRenderer> create();

        virtual void drawIndexed(Ref<VertexArray> vao, Ref<IndexBuffer> ibo, Ref<Shader> shader) = 0;
    };
}

#endif
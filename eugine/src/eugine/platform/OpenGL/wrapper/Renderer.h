//
// Created by aiden on 4/23/22.
//

#ifndef EUGINE_RENDERER_H
#define EUGINE_RENDERER_H

#include "eugine/rendering/LowLevelRenderer.h"

#include "VertexArray.h"
#include "Shader.h"
#include "IndexBuffer.h"

namespace eg {
    namespace GLWrapper {
        class Renderer : public ::eg::rendering::LowLevelRenderer {
        public:
            Renderer();
            ~Renderer();

            void drawIndexed(Ref<eg::rendering::VertexArray> vertexArray, 
                             Ref<eg::rendering::IndexBuffer> indexBuffer, 
                             Ref<eg::rendering::Shader> shader) override;
        private:
        };
    }
}

#endif //EUGINE_RENDERER_H

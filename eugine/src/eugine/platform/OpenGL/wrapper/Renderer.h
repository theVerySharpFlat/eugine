//
// Created by aiden on 4/23/22.
//

#ifndef EUGINE_RENDERER_H
#define EUGINE_RENDERER_H

#include "VertexArray.h"
#include "Shader.h"

namespace eg {
    namespace GLWrapper {
        class Renderer {
        public:
            Renderer();
            ~Renderer();

            void draw(const VertexArray& vertexArray, Shader& shader);
        private:
        };
    }
}

#endif //EUGINE_RENDERER_H

//
// Created by aiden on 4/23/22.
//

#include "Renderer.h"

#include "error.h"
#include <glad/glad.h>

namespace eg {
    namespace GLWrapper {
        Renderer::Renderer() {

        }

        Renderer::~Renderer() {

        }

        void Renderer::draw(const VertexArray &vertexArray, Shader &shader) {
            vertexArray.bind();
            shader.bind();
            GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));
        }
    }
}
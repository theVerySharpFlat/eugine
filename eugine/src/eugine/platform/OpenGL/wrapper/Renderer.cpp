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

        void Renderer::draw(const VertexArray &vertexArray, const IndexBuffer& indexBuffer, const Shader &shader) {
            vertexArray.bind();
            shader.bind();
            indexBuffer.bind();
            GLCall(glDrawElements(GL_TRIANGLES, indexBuffer.getElementCount(), GL_UNSIGNED_INT, NULL));
        }
    }
}
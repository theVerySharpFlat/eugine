//
// Created by aiden on 4/23/22.
//

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "error.h"
#include "eugine/platform/OpenGL/Shader.h"
#include "eugine/rendering/Types.h"
#include "eugine/rendering/VertexBuffer.h"

#include "glad/glad.h"

namespace eg::rendering::GLWrapper {

    static u32 mapShaderTypeToGLType(rendering::ShaderType type) {
        switch (type) {
            case rendering::SHDR_FLOAT:
                return GL_FLOAT;
            case rendering::SHDR_BOOL:
                return GL_BOOL;
            case rendering::SHDR_INT:
                return GL_INT;
            case rendering::SHDR_DOUBLE:
                return GL_DOUBLE;
            case rendering::SHDR_UINT:
                return GL_UNSIGNED_INT;

            case rendering::SHDR_VEC2:
                return GL_FLOAT;
            case rendering::SHDR_VEC3:
                return GL_FLOAT;
            case rendering::SHDR_VEC4:
                return GL_FLOAT;

            case rendering::SHDR_MAT2:
                return GL_FLOAT;
            case rendering::SHDR_MAT3:
                return GL_FLOAT;
            case rendering::SHDR_MAT4:
                return GL_FLOAT;
        }
        error("unknown shader type: {}", (i32) type);
        return 0;
    }

    static u32 mapShaderTypeToGLTypeCount(rendering::ShaderType type) {
        switch (type) {
            case rendering::SHDR_FLOAT:
                return 1;
            case rendering::SHDR_BOOL:
                return 1;
            case rendering::SHDR_INT:
                return 1;
            case rendering::SHDR_DOUBLE:
                return 1;
            case rendering::SHDR_UINT:
                return 1;

            case rendering::SHDR_VEC2:
                return 2;
            case rendering::SHDR_VEC3:
                return 3;
            case rendering::SHDR_VEC4:
                return 4;

            case rendering::SHDR_MAT2:
                return 2 * 2;
            case rendering::SHDR_MAT3:
                return 3 * 3;
            case rendering::SHDR_MAT4:
                return 4 * 4;
        }
        error("unknown shader type: {}", (i32) type);
        return 0;
    }


    VertexArray::VertexArray() {
        GLCall(glCreateVertexArrays(1, &m_ID));
    }

    VertexArray::VertexArray(GLWrapper::VertexBuffer& vbo) : VertexArray() {
        _setBuffer(vbo);
    }

    VertexArray::~VertexArray() {
        GLCall(glDeleteVertexArrays(1, &m_ID));
    }

    void VertexArray::setBuffer(GLWrapper::VertexBuffer& vertexBuffer) {
        _setBuffer(vertexBuffer);
    }


    void VertexArray::_setBuffer(GLWrapper::VertexBuffer& vertexBuffer) {
        bind();
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer.m_ID));

        const auto vertexBufferLayout = vertexBuffer.getLayout();
        // trace("stride: {}", vertexBufferLayout.getStride());

        u64 offset = 0;
        const auto& attributes = vertexBufferLayout.getAttributes();
        for (int i = 0; i < attributes.size(); i++) {
            ShaderType t = attributes[i].type;
            if(t == SHDR_UINT || t == SHDR_INT) {
              glVertexAttribIPointer(i, mapShaderTypeToGLTypeCount(attributes[i].type) * attributes[i].count, mapShaderTypeToGLType(attributes[i].type), vertexBufferLayout.getStride(), (void*)offset);
            } else {
              GLCall(glVertexAttribPointer(i, mapShaderTypeToGLTypeCount(attributes[i].type) * attributes[i].count,
                                           mapShaderTypeToGLType(attributes[i].type), GL_FALSE,
                                           vertexBufferLayout.getStride(),
                                           (void*) offset));
            }
            GLCall(glEnableVertexAttribArray(i));
            trace("enableVertexAttribArray({})", i);
            trace("stride: {}", vertexBufferLayout.getStride());

            offset += attributes[i].count * getSizeOfType(attributes[i].type);
        }
    }

    void VertexArray::bind() {
        GLCall(glBindVertexArray(m_ID));
    }

    void VertexArray::unBind() {
        GLCall(glBindVertexArray(0));
    }
}

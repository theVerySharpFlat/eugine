#version 450

#define EG_VULKAN

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec4 color;
layout(location = 3) in float fragmentAlphaBlendFactor;
layout(location = 4) in uint textureIndex;

layout(location = 0) out vec4 out_color;
layout(location = 1) out float out_fragmentAlphaBlendFactor;
layout(location = 2) out uint out_textureIndex;
layout(location = 3) out vec2 out_texCoord;

#ifdef EG_VULKAN
layout(binding = 0, set = 0) uniform PerFrameUBO {
    mat4 projxview;
};
#else
//TODO: opengl version of uniforms in 2d quad rendering shader
#endif


void main() {
    gl_Position = projxview * vec4(position, 0.0, 1.0);
    gl_Position.y *= -1;

    out_color = color;
    out_fragmentAlphaBlendFactor = fragmentAlphaBlendFactor;
    out_textureIndex = textureIndex;
    out_texCoord = texCoord;
}
#version 450

#define EG_VULKAN

layout(location = 0) in vec4 color;
layout(location = 1) in float fragmentAlphaBlendFactor;
layout(location = 2) flat in uint textureIndex;
layout(location = 3) flat in vec2 texCoord;

layout(location = 0) out vec4 fragColor;

void main() {
    fragColor = color;
}
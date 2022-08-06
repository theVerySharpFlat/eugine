#version 450

#define EG_VULKAN

layout(location = 0) in vec4 color;
layout(location = 1) in float fragmentAlphaBlendFactor;
layout(location = 2) flat in uint textureIndex;
layout(location = 3) in vec2 texCoord;

layout(location = 0) out vec4 fragColor;

layout(set = 1, binding = 0) uniform sampler2D samplers[128];

void main() {
    fragColor = /*vec4(texCoord, 1.0f, 1.0f);*/texture(samplers[textureIndex], texCoord);
}
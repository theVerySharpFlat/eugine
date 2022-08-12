#version 450

layout(location = 0) in vec4 color;
layout(location = 1) in float fragmentAlphaBlendFactor;
layout(location = 2) flat in uint textureIndex;
layout(location = 3) in vec2 texCoord;

layout(location = 0) out vec4 fragColor;

#ifdef EG_VULKAN
layout(set = 1, binding = 0) uniform sampler2D samplers[128];
#else
uniform sampler2D samplers[32];
#endif

void main() {
    vec4 texelColor = texture(samplers[textureIndex], texCoord);
    fragColor = mix(texelColor, color, color.a);
    fragColor.a = texelColor.a;
    fragColor.a *= fragmentAlphaBlendFactor;
}

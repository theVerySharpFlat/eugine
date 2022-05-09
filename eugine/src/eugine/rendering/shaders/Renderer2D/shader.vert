#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in float texIndex;

out vec3 ourColor;

uniform mat4 projxview;

void main() {
	gl_Position = projxview * vec4(aPos, 0.0, 1.0);
	ourColor = aColor;
}
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 atexCoords;
layout (location = 3) in float atexIndex;

out vec2 texCoords;
out vec4 ourColor;
out float texIndex;

uniform mat4 projxview;

void main() {
	ourColor = aColor;
	texCoords = atexCoords;
	texIndex = atexIndex;
	gl_Position = projxview * vec4(aPos, 0.0, 1.0);
}
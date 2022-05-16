#version 330 core
out vec4 FragColor;

in vec2 texCoords;
in float texIndex;
in vec3 ourColor;

uniform sampler2D samplers[32];

void main() {
	switch(int(texIndex)) {
		case 0: FragColor = texture(samplers[0], texCoords); break;
		case 1: FragColor = texture(samplers[1], texCoords); break;
		case 2: FragColor = texture(samplers[2], texCoords); break;
		case 3: FragColor = texture(samplers[3], texCoords); break;
		case 4: FragColor = texture(samplers[4], texCoords); break;
		case 5: FragColor = texture(samplers[5], texCoords); break;
		case 6: FragColor = texture(samplers[6], texCoords); break;
		case 7: FragColor = texture(samplers[7], texCoords); break;
		case 8: FragColor = texture(samplers[8], texCoords); break;
		case 9: FragColor = texture(samplers[9], texCoords); break;
		case 10: FragColor = texture(samplers[10], texCoords); break;
		case 11: FragColor = texture(samplers[11], texCoords); break;
		case 12: FragColor = texture(samplers[12], texCoords); break;
		case 13: FragColor = texture(samplers[13], texCoords); break;
		case 14: FragColor = texture(samplers[14], texCoords); break;
		case 15: FragColor = texture(samplers[15], texCoords); break;
		case 16: FragColor = texture(samplers[16], texCoords); break;
		case 17: FragColor = texture(samplers[17], texCoords); break;
		case 18: FragColor = texture(samplers[18], texCoords); break;
		case 19: FragColor = texture(samplers[19], texCoords); break;
		case 20: FragColor = texture(samplers[20], texCoords); break;
		case 21: FragColor = texture(samplers[21], texCoords); break;
		case 22: FragColor = texture(samplers[22], texCoords); break;
		case 23: FragColor = texture(samplers[23], texCoords); break;
		case 24: FragColor = texture(samplers[24], texCoords); break;
		case 25: FragColor = texture(samplers[25], texCoords); break;
		case 26: FragColor = texture(samplers[26], texCoords); break;
		case 27: FragColor = texture(samplers[27], texCoords); break;
		case 28: FragColor = texture(samplers[28], texCoords); break;
		case 29: FragColor = texture(samplers[29], texCoords); break;
		case 30: FragColor = texture(samplers[30], texCoords); break;
		case 31: FragColor = texture(samplers[31], texCoords); break;
	}
}
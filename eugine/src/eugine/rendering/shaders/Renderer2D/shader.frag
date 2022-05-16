#version 330 core
out vec4 FragColor;

in vec2 texCoords;
in float texIndex;
in vec3 ourColor;

uniform sampler2D u_textures[32];

uniform sampler2D u_texture0;
uniform sampler2D u_texture1;
uniform sampler2D u_texture2;
uniform sampler2D u_texture3;
uniform sampler2D u_texture4;
uniform sampler2D u_texture5;
uniform sampler2D u_texture6;
uniform sampler2D u_texture7;
uniform sampler2D u_texture8;
uniform sampler2D u_texture9;
uniform sampler2D u_texture10;
uniform sampler2D u_texture11;
uniform sampler2D u_texture12;
uniform sampler2D u_texture13;
uniform sampler2D u_texture14;
uniform sampler2D u_texture15;
uniform sampler2D u_texture16;
uniform sampler2D u_texture17;
uniform sampler2D u_texture18;
uniform sampler2D u_texture19;
uniform sampler2D u_texture20;
uniform sampler2D u_texture21;
uniform sampler2D u_texture22;
uniform sampler2D u_texture23;
uniform sampler2D u_texture24;
uniform sampler2D u_texture25;
uniform sampler2D u_texture26;
uniform sampler2D u_texture27;
uniform sampler2D u_texture28;
uniform sampler2D u_texture29;
uniform sampler2D u_texture30;
uniform sampler2D u_texture31;


void main() {
	switch(int(texIndex)) {
		case 0: FragColor = texture(u_texture0, texCoords); break;
		case 1: FragColor = texture(u_texture1, texCoords); break;
		case 2: FragColor = texture(u_texture2, texCoords); break;
		case 3: FragColor = texture(u_texture3, texCoords);break;
		case 4: FragColor = texture(u_texture4, texCoords);break;
		case 5: FragColor = texture(u_texture5, texCoords);break;
		case 6: FragColor = texture(u_texture6, texCoords);break;
		case 7: FragColor = texture(u_texture7, texCoords);break;
		case 8: FragColor = texture(u_texture8, texCoords);break;
		case 9: FragColor = texture(u_texture9, texCoords);break;
		case 10: FragColor = texture(u_texture10, texCoords);break;
		case 11: FragColor = texture(u_texture11, texCoords);break;
		case 12: FragColor = texture(u_texture12, texCoords);break;
		case 13: FragColor = texture(u_texture13, texCoords);break;
		case 14: FragColor = texture(u_texture14, texCoords);break;
		case 15: FragColor = texture(u_texture15, texCoords);break;
		case 16: FragColor = texture(u_texture16, texCoords);break;
		case 17: FragColor = texture(u_texture17, texCoords);break;
		case 18: FragColor = texture(u_texture18, texCoords);break;
		case 19: FragColor = texture(u_texture19, texCoords);break;
		case 20: FragColor = texture(u_texture20, texCoords);break;
		case 21: FragColor = texture(u_texture21, texCoords);break;
		case 22: FragColor = texture(u_texture22, texCoords);break;
		case 23: FragColor = texture(u_texture23, texCoords);break;
		case 24: FragColor = texture(u_texture24, texCoords);break;
		case 25: FragColor = texture(u_texture25, texCoords);break;
		case 26: FragColor = texture(u_texture26, texCoords);break;
		case 27: FragColor = texture(u_texture27, texCoords);break;
		case 28: FragColor = texture(u_texture28, texCoords);break;
		case 29: FragColor = texture(u_texture29, texCoords);break;
		case 30: FragColor = texture(u_texture30, texCoords);break;
		case 31: FragColor = texture(u_texture31, texCoords);break;
	}
}
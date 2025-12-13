#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in int aNormal;
layout (location = 2) in vec2 aTex;

out vec3 Normal;
out vec2 texCoord;
out vec4 fragPosLight;

uniform mat4 camMatrix;
uniform mat4 lightProjection;

void main() {
	switch (aNormal) {
	case 0:
	    Normal = vec3(0, 1, 0);
	    break;
	case 1:
	    Normal = vec3(0, -1, 0);
	    break;
	case 2:
	    Normal = vec3(0, 0, 1);
	    break;
	case 3:
	    Normal = vec3(1, 0, 0);
	    break;
	case 4:
	    Normal = vec3(0, 0, -1);
	    break;
	case 5:
	    Normal = vec3(-1, 0, 0);
	    break;
	}

	// if(aNormal == 0)
	// Normal = aNormal;

	texCoord = mat2(0.0, -1.0, 1.0, 0.0) * aTex;
	fragPosLight = lightProjection * vec4(aPos, 1.0f);
	gl_Position = camMatrix * vec4(aPos, 1.0);
}
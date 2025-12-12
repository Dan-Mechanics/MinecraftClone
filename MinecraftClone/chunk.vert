#version 330 core

layout (location = 0) in ivec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;

out vec3 Normal;
out vec2 texCoord;
out vec4 fragPosLight;

uniform mat4 camMatrix;
uniform mat4 lightProjection;

void main() {
	Normal = aNormal;

	texCoord = mat2(0.0, -1.0, 1.0, 0.0) * aTex;

	fragPosLight = lightProjection * vec4(crntPos, 1.0f);
	
	gl_Position = camMatrix * vec4(aPos, 1.0);
}
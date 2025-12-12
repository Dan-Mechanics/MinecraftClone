#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 3) in vec2 aTex;

out vec3 crntPos;
out vec3 Normal;
out vec2 texCoord;
out vec4 fragPosLight;

uniform mat4 camMatrix;
uniform mat4 model;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;
uniform mat4 lightProjection;

void main() {
	crntPos = vec3(model * translation * scale * rotation * vec4(aPos, 1.0f));
	Normal = vec3(rotation * vec4(aNormal, 1.0f));

	texCoord = mat2(0.0, -1.0, 1.0, 0.0) * aTex;

	fragPosLight = lightProjection * vec4(crntPos, 1.0f);
	
	gl_Position = camMatrix * vec4(crntPos, 1.0);
}
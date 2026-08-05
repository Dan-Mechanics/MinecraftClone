#version 460 core

layout (location = 0) in vec3 aPos;

uniform mat4 lightProjection;

void main()
{
	gl_Position = lightProjection * vec4(aPos, 1.0);
}
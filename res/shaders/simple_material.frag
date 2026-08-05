#version 460 core

out vec4 FragColor;

in vec3 Normal;
in vec2 texCoord;

uniform sampler2D diffuse0;
uniform vec4 lightColor;
uniform vec4 worldColor;
uniform vec3 lightPos;

vec4 directionalLight() {
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	vec4 light = diffuse * lightColor + worldColor;
	light.x = min(light.x, 1.0f);
	light.y = min(light.y, 1.0f);
	light.z = min(light.z, 1.0f);

	return texture(diffuse0, texCoord) * light;
}

void main() 
{
	 FragColor = directionalLight();
}
#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec2 texCoord;
in vec4 fragPosLight;

uniform sampler2D diffuse0;
uniform sampler2D shadowMap;
uniform vec4 lightColor;
uniform vec4 worldColor;
uniform vec3 lightPos;
uniform vec3 camPos;

vec4 directionalLight() {
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	float shadow = 0.0f;
	vec3 lightCoords = fragPosLight.xyz / fragPosLight.w;
	if (lightCoords.z <= 1.0f)
	{
		lightCoords = (lightCoords + 1.0f) / 2.0f;
		
		float closestDepth = texture(shadowMap, lightCoords.xy).r;
		float currentDepth = lightCoords.z;

		float bias = max(0.025f * (1.0f - dot(normal, lightDirection)), 0.0005f);
		if (currentDepth > closestDepth + bias)
			shadow = 1.0f;
	}

	vec4 light = diffuse * (1.0f - shadow) * lightColor + worldColor;
	light.x = min(light.x, 1.0f);
	light.y = min(light.y, 1.0f);
	light.z = min(light.z, 1.0f);

	return texture(diffuse0, texCoord) * light;
}

void main() {
	FragColor = directionalLight();
}
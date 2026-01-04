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

	return worldColor;
}

float near = 0.1f;
float far = 100.0f;

float linearizeDepth(float depth)
{
	return (2.0 * near * far) / (far + near - (depth * 2.0 - 1.0) * (far - near));
}

float logisticDepth(float depth, float steepness, float offset)
{
	float zVal = linearizeDepth(depth);
	return (1 / (1 + exp(-steepness * (zVal - offset))));
}

void main()
{
	 vec4 color = texture(diffuse0, texCoord);
	 color.a = 0.75f;
	 if (color.a < 0.1f)
		discard;
	 
	 color *= worldColor;
	 float depth = logisticDepth(gl_FragCoord.z, 0.22f, 78.0f);
	 color = color * (1.0f - depth) + depth * worldColor;
	 //color.a = 0.5f;

	 FragColor = color;
}
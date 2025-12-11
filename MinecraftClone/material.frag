#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;

// Imports the current position from the Vertex Shader
in vec3 crntPos;
// Imports the normal from the Vertex Shader
in vec3 Normal;
// Imports the color from the Vertex Shader
in vec3 color;
// Imports the texture coordinates from the Vertex Shader
in vec2 texCoord;
// Imports the fragment position of the light
in vec4 fragPosLight;



// Gets the Texture Units from the main function
uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform sampler2D shadowMap;
// Gets the color of the light from the main function
uniform vec4 lightColor;
uniform vec4 worldColor;
// Gets the position of the light from the main function
uniform vec3 lightPos;
// Gets the position of the camera from the main function
uniform vec3 camPos;


vec4 shadowWorldLight()
{
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	float shadow = 0.0f;
	vec3 lightCoords = fragPosLight.xyz / fragPosLight.w;
	if(lightCoords.z <= 1.0f)
	{
		lightCoords = (lightCoords + 1.0f) / 2.0f;
		
		float closestDepth = texture(shadowMap, lightCoords.xy).r;
		float currentDepth = lightCoords.z;

		float bias = max(0.025f * (1.0f - dot(normal, lightDirection)), 0.0005f);
		if (currentDepth > closestDepth + bias)
			shadow = 1.0f;
	}


	vec4 light = texture(specular0, texCoord).r * diffuse * (1.0f - shadow) * lightColor + worldColor;
	light.x = min(light.x, 1.0f);
	light.y = min(light.y, 1.0f);
	light.z = min(light.z, 1.0f);

	return texture(diffuse0, texCoord) * light;
}

void main()
{
	// outputs final color
	FragColor = shadowWorldLight();
}
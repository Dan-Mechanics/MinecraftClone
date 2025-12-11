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



// Gets the Texture Units from the main function
uniform sampler2D diffuse0;
uniform sampler2D specular0;
// Gets the color of the light from the main function
uniform vec4 lightColor;
uniform vec4 selfColor;
uniform vec4 worldColor;

// Gets the position of the light from the main function
uniform vec3 lightPos;
// Gets the position of the camera from the main function
uniform vec3 camPos;

vec4 worldLight()
{
	vec3 lightVec = lightPos - crntPos;
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightVec);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// vec4 light = texture(specular0, texCoord).r * diffuse * lightColor + worldColor;
	vec4 light = diffuse * lightColor + worldColor;
	light.x = min(light.x, 1.0f);
	light.y = min(light.y, 1.0f);
	light.z = min(light.z, 1.0f);

	return selfColor * light;
}

void main()
{
	// outputs final color
	FragColor = worldLight();
}
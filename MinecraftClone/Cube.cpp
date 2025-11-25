#include "Cube.h"

Cube::Cube() = default;
Cube::Cube(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale) : pos{ pos }, rot{ rot }, scale{ scale } {
	Vertex cubeVerts[] = {
		Vertex{glm::vec3(-0.5f, -0.5f, 0),
		Vertex{glm::vec3(0.5f, -0.5f, 0), 
		Vertex{glm::vec3(0.5f, 0.5f, 0), 
		Vertex{glm::vec3(-0.5f, 0.5f, 0)
	};

	GLuint cubeTris[] = {
		0, 2, 1,
		0, 3, 2
	};
	
	std::vector <Vertex> verts(cubeVerts, cubeVerts + sizeof(cubeVerts) / sizeof(Vertex));
	std::vector <GLuint> tris(cubeTris, cubeTris + sizeof(cubeTris) / sizeof(GLuint));

	Texture textures[]{
		Texture("planks.png", "diffuse", 0),
		Texture("planksSpec.png", "specular", 1)
	};
	std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));

	faceMesh = { verts, tris, tex };
}

void Cube::draw(const Shader& shader, const Camera& camera, const glm::vec3& lightPos, const glm::vec4& lightColor) const {
	glm::mat4 matrix{ 1.0f };
	glm::quat rotation{ 1.0f, 0.0f, 0.0f, 0.0f };

	faceMesh.draw(shader, camera, matrix, pos, rotation, scale, lightPos, lightColor);
	faceMesh.draw(shader, camera, matrix, pos + glm::vec3(0.0f, 0.0f, -1.0f), { -1.0f, 0.0f, 0.0f, 0.0f }, scale, lightPos, lightColor);

}

void Cube::move(const glm::vec3& vel, const double dt) {
	pos += vel * (float)dt;
}

void Cube::free() const {
	faceMesh.free();
}
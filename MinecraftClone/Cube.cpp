#include "Cube.h"

Vertex cubeVerts[] = { 
	//     COORDINATES     //
	Vertex{glm::vec3(-0.5f, -0.5f,  0.5f)},
	Vertex{glm::vec3(-0.5f, -0.5f, -0.5f)},
	Vertex{glm::vec3(0.5f, -0.5f, -0.5f)},
	Vertex{glm::vec3(0.5f, -0.5f,  0.5f)},
	Vertex{glm::vec3(-0.5f,  0.5f,  0.5f)},
	Vertex{glm::vec3(-0.5f,  0.5f, -0.5f)},
	Vertex{glm::vec3(0.5f,  0.5f, -0.5f)},
	Vertex{glm::vec3(0.5f,  0.5f,  0.5f)}
};

GLuint cubeTris[] = {
	0, 1, 3, 3, 1, 2,
	1, 5, 2, 2, 5, 6,
	5, 4, 6, 6, 4, 7,
	4, 0, 7, 7, 0, 3,
	3, 2, 7, 7, 2, 6,
	4, 5, 0, 0, 5, 1
};

Cube::Cube() = default;
Cube::Cube(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale) : pos{ pos }, rot{ rot }, scale{ scale } {
	std::vector <Vertex> verts(cubeVerts, cubeVerts + sizeof(cubeVerts) / sizeof(Vertex));
	std::vector <GLuint> tris(cubeTris, cubeTris + sizeof(cubeTris) / sizeof(GLuint));
	std::vector<Texture> tex{};
	mesh = { verts, tris, tex };
}

void Cube::draw(const Shader& shader, const Camera& camera) {
	while (rot.x >= 360.0f) {
		rot.x -= 360.0f;
	}
	while (rot.y >= 360.0f) {
		rot.y -= 360.0f;
	}
	while (rot.y >= 360.0f) {
		rot.y -= 360.0f;
	}

	while (rot.x < 0.0f) {
		rot.x += 360.0f;
	}
	while (rot.y < 0.0f) {
		rot.y += 360.0f;
	}
	while (rot.y < 0.0f) {
		rot.y += 360.0f;
	}

	glm::vec3 right{ 1.0f, 0.0f, 0.0f };
	glm::vec3 up{ 0.0f, 1.0f, 0.0f };
	glm::vec3 forward{ 0.0f, 0.0f, 1.0f };

	glm::vec3 direction{ 0.0f, 0.0f, 1.0f };
	direction = glm::rotate(direction, glm::radians(rot.x), right);
	direction = glm::rotate(direction, glm::radians(rot.y), up);
	direction = glm::rotate(direction, glm::radians(rot.z), forward);

	glm::quat rotation = glm::quatLookAt(direction, up);

	glm::mat4 matrix{ 1.0f };
	mesh.draw(shader, camera, matrix, pos, rotation, scale, { 0.0f, 0.0f, 0.0f }, color);
}

void Cube::move(const glm::vec3& vel, const double dt) {
	pos += vel * (float)dt;
}

void Cube::setColor(const glm::vec4& color) {
	this->color = color;
}

void Cube::free() const {
	mesh.free();
}
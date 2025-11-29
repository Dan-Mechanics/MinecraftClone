#include "Cube.h"

Cube::Cube() = default;
Cube::Cube(std::vector<Vertex>& verts, std::vector<GLuint>& tris, const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale) : pos{ pos }, rot{ rot }, scale{ scale } {
	std::vector<Texture> tex{};
	mesh = { verts, tris, tex };

	// DEFAULT IS MAGENTA.
	setColor(glm::vec4{ 1.0f, 0.0f, 1.0f, 1.0f });
}

Cube::Cube(std::vector<Vertex>& verts, std::vector<GLuint>& tris, std::vector<Texture>& tex, const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale) {
	mesh = { verts, tris, tex };

	// DEFAULT IS MAGENTA.
	setColor(glm::vec4{ 1.0f, 0.0f, 1.0f, 1.0f });
}

void Cube::draw(const Shader& shader, const Camera& camera, glm::vec4 lightColor, glm::vec3 lightPos, glm::vec4 worldColor) {
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

	//glm::mat4 matrix{ 1.0f };
	mesh.draw(shader, camera, modelMatrix, pos, rotation, scale, lightPos, lightColor, worldColor);
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
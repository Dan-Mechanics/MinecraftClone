#include "Object.h"

Object::Object() = default;
Object::Object(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale) : pos{ pos }, rot{ rot },
scale{ scale }, color{ glm::vec4{1.0f, 0.0f, 1.0f, 1.0f} } { }

void Object::drawWithMaterial(const Mesh& mesh, const std::vector<Texture>& material, const Shader& shader, const Camera& camera, const glm::vec4& lightColor, const glm::vec3& lightPos, const glm::vec4& worldColor) {
	// glFrontFace(withTheClock ? GL_CW : GL_CCW);
	glm::quat rotation = calculateQuat();
	mesh.drawTexture(shader, camera, pos, rotation, scale, lightPos, lightColor, worldColor, material);
}

void Object::drawAsColor(const Mesh& mesh, const Shader& shader, const Camera& camera,
	const glm::vec4& lightColor, const glm::vec3& lightPos, const glm::vec4& worldColor) {
	glm::quat rotation = calculateQuat();
	mesh.drawColor(shader, camera, pos, rotation, scale, lightPos, lightColor, worldColor, color);
}

void Object::drawAsUnlitColor(const Mesh& mesh, const Shader& shader, const Camera& camera) {
	glm::quat rotation = calculateQuat();
	mesh.drawUnlit(shader, camera, pos, rotation, scale, color);
}

void Object::move(const glm::vec3& vel, const float dt) {
	pos += vel * dt;
}

void Object::rotate(const glm::vec3& vel, const float dt) {
	rot += vel * dt;
}

void Object::setColor(const glm::vec4& color) {
	this->color = color;
}

glm::quat Object::calculateQuat() {
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
	return rotation;
}

#include "Object.h"

Object::Object() : visible{ true } {
	setScale(glm::vec3{ 1.0f });
	setColor(glm::vec4{ 1.0f, 0.0f, 1.0f, 1.0f });
}

void Object::drawWithMaterial(const Mesh& mesh, const std::vector<Texture>& material, const Shader& shader, const Camera& camera, const glm::vec4& lightColor, const glm::vec3& lightPos, const glm::vec4& worldColor) {
	if (!visible)
		return;

	// glFrontFace(withTheClock ? GL_CW : GL_CCW);
	glm::quat rotation = calculateQuat();
	mesh.drawTexture(shader, camera, pos, rotation, scale, lightPos, lightColor, worldColor, material);
}

void Object::drawAsColor(const Mesh& mesh, const Shader& shader, const Camera& camera,
	const glm::vec4& lightColor, const glm::vec3& lightPos, const glm::vec4& worldColor) {
	if (!visible)
		return;

	glm::quat rotation = calculateQuat();
	mesh.drawColor(shader, camera, pos, rotation, scale, lightPos, lightColor, worldColor, color);
}

void Object::drawAsUnlitColor(const Mesh& mesh, const Shader& shader, const Camera& camera) {
	if (!visible)
		return;

	glm::quat rotation = calculateQuat();
	mesh.drawUnlit(shader, camera, pos, rotation, scale, color);
}

void Object::moveOverTime(const glm::vec3& vel, const float dt) {
	pos += vel * dt;
}

void Object::rotateOverTime(const glm::vec3& angVel, const float dt) {
	rot += angVel * dt;
}

void Object::setPos(const glm::vec3& to) {
	pos = to;
}

void Object::setRot(const glm::vec3& to) {
	rot = to;
}

void Object::setScale(const glm::vec3& to) {
	scale = to;
}

void Object::setColor(const glm::vec4& to) {
	color = to;
}

void Object::setVisible(const bool to) {
	visible = to;
}

void Object::setAs(const Object& other) {
	setPos(other.pos);
	setRot(other.rot);
	setScale(other.scale);
	setColor(other.color);
	setVisible(other.visible);
}

glm::quat Object::calculateQuat() {
	while (rot.x >= 360.0f) {
		rot.x -= 360.0f;
	}

	while (rot.y >= 360.0f) {
		rot.y -= 360.0f;
	}

	while (rot.z >= 360.0f) {
		rot.z -= 360.0f;
	}

	while (rot.x < 0.0f) {
		rot.x += 360.0f;
	}

	while (rot.y < 0.0f) {
		rot.y += 360.0f;
	}

	while (rot.z < 0.0f) {
		rot.z += 360.0f;
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

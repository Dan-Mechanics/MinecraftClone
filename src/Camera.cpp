#include "Camera.h"

Camera::Camera() = default;

void Camera::updateMatrix(const float fovDeg, const float nearPlane, const float farPlane, const glm::vec3& pos,
	const glm::vec3& forward, const unsigned int width, const unsigned int height) {
	position = pos;

	auto view = glm::mat4{ 1.0f };
	auto projection = glm::mat4{ 1.0f };

	view = glm::lookAt(pos, pos + forward, worldUp);
	projection = glm::perspective(glm::radians(fovDeg), (float)width / height, nearPlane, farPlane);

	matrix = projection * view;
}

void Camera::sendMatrixToShader(const Shader& shader, const char* uniform) const {
	glUniformMatrix4fv(glGetUniformLocation(shader.id, uniform), 1, GL_FALSE, glm::value_ptr(matrix));
}
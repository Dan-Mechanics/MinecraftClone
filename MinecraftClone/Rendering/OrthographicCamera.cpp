#include "OrthographicCamera.h"

OrthographicCamera::OrthographicCamera() = default;

void OrthographicCamera::updateMatrix(const float left, const float right, const float bottom, const float top, const float zNear, const float zFar, const glm::vec3& pos, const glm::vec3& forward) {
	position = pos;

	// auto view = glm::mat4{ 1.0f };
	// auto projection = glm::mat4{ 1.0f };

	auto view = glm::lookAt(pos, pos + forward, worldUp);
	glm::mat4 orthgonalProjection = glm::ortho(left, right, bottom, top, zNear, zFar);

	matrix = orthgonalProjection * view;
}

void OrthographicCamera::sendMatrixToShader(const Shader& shader, const char* uniform) const {
	glUniformMatrix4fv(glGetUniformLocation(shader.id, uniform), 1, GL_FALSE, glm::value_ptr(matrix));
}
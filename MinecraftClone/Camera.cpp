#include "Camera.h"

Camera::Camera(int width, int height, glm::vec3 position) : width{ width }, height{ height }, position{ position } { }

void Camera::sendMatrix(float fovDeg, float nearPlane, float farPlane, Shader& shader, const char* uniform) const {
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	view = glm::lookAt(position, position + orientation, up);
	projection = glm::perspective(glm::radians(fovDeg), (float)width / height, nearPlane, farPlane);

	glUniformMatrix4fv(glGetUniformLocation(shader.id, uniform), 1, GL_FALSE, glm::value_ptr(projection * view));
}

void Camera::handleInput() {
	// ...
}

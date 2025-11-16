#include "Camera.h"

Camera::Camera(int width, int height, glm::vec3 position) : width{ width }, height{ height }, position{ position } { }

void Camera::updateMatrix(float fovDeg, float nearPlane, float farPlane) {
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	view = glm::lookAt(position, position + orientation, up);
	projection = glm::perspective(glm::radians(fovDeg), (float)width / height, nearPlane, farPlane);

	cameraMatrix = projection * view;
}

void Camera::applyMatrix(Shader& shader, const char* uniform) const {
	glUniformMatrix4fv(glGetUniformLocation(shader.id, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::moveCamera(GLFWwindow* window) {
	// WASD. ===
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
		position += speed * orientation;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) 
		position += speed * -glm::normalize(glm::cross(orientation, up));

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) 
		position += speed * -orientation;

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) 
		position += speed * glm::normalize(glm::cross(orientation, up));

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) 
		position += speed * up;

	// UP DOWN. ===
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) 
		position += speed * up;

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) 
		position += speed * -up;

	// SPRINT. ===
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		speed = 0.004f;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE) {
		speed = 0.001f;
	}

	// CURSOR. ===

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		if (firstClick)
		{
			glfwSetCursorPos(window, width / 2, height / 2);
			firstClick = false;
		}

		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (height / 2)) / height;

		glm::vec3 newOrientation = glm::rotate(orientation, glm::radians(-rotX), glm::normalize(glm::normalize(glm::cross(orientation, up))));

		if (abs(glm::angle(newOrientation, up) - glm::radians(90.0f)) <= glm::radians(85.0f))
			orientation = newOrientation;

		orientation = glm::rotate(orientation, glm::radians(-rotY), up);

		glfwSetCursorPos(window, width / 2, height / 2);
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstClick = true;
	}
}

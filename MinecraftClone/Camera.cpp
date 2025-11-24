#include "Camera.h"

Camera::Camera() = default;

Camera::Camera(const unsigned int width, const unsigned int height, const float standardSpeed, const float sensitivity) :
	width{ width }, height{ height }, standardSpeed{ standardSpeed }, sensitivity{ sensitivity } { }

void Camera::updateMatrix(float fovDeg, float nearPlane, float farPlane) {
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	view = glm::lookAt(position, position + eyesForward, up);
	projection = glm::perspective(glm::radians(fovDeg), (float)width / height, nearPlane, farPlane);

	cameraMatrix = projection * view;
}

void Camera::sendMatrixToShader(Shader& shader, const char* uniform) const {
	glUniformMatrix4fv(glGetUniformLocation(shader.id, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::moveCamera(GLFWwindow* window, const float tickInterval) {
	glm::vec3 movement{};
	
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		movement += bodyForward;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		movement -= glm::normalize(glm::cross(bodyForward, up));

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		movement -= bodyForward;

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		movement += glm::normalize(glm::cross(bodyForward, up));

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		movement += up;

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		movement += up;

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		movement -= up;

	const auto currentSpeed = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ? standardSpeed * 3.0f : standardSpeed;
	position += tickInterval * currentSpeed * movement;
}

void Camera::rotateCamera(GLFWwindow* window) {
	const auto leftIsPressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

	if (!leftIsPressed) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		shouldCenterCursor = true;
		return;
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	if (shouldCenterCursor) {
		glfwSetCursorPos(window, (double)width / 2, (double)height / 2);
		shouldCenterCursor = false;
	}

	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);

	float rotX = sensitivity * (float)(mouseY - ((float)height / 2)) / height;
	float rotY = sensitivity * (float)(mouseX - ((float)height / 2)) / height;

	glm::vec3 newOrientation = glm::rotate(eyesForward, glm::radians(-rotX), glm::normalize(glm::normalize(glm::cross(eyesForward, up))));

	if (abs(glm::angle(newOrientation, up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		eyesForward = newOrientation;

	eyesForward = glm::rotate(eyesForward, glm::radians(-rotY), up);
	bodyForward = glm::rotate(bodyForward, glm::radians(-rotY), up);

	glfwSetCursorPos(window, (double)width / 2, (double)height / 2);
}
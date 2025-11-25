#include "Camera.h"

Camera::Camera() = default;

Camera::Camera(GLFWwindow* window, const unsigned int width, const unsigned int height, const float standardSpeed, const float sensitivity) :
	width{ width }, height{ height }, standardSpeed{ standardSpeed }, sensitivity{ sensitivity } { 
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetCursorPos(window, (double)width / 2.0, (double)height / 2.0f);
}

void Camera::updateMatrix(float fovDeg, float nearPlane, float farPlane) {
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	view = glm::lookAt(position, position + eyesForward, up);
	projection = glm::perspective(glm::radians(fovDeg), (float)width / height, nearPlane, farPlane);

	cameraMatrix = projection * view;
}

void Camera::sendMatrixToShader(const Shader& shader, const char* uniform) const {
	glUniformMatrix4fv(glGetUniformLocation(shader.id, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::moveCamera(GLFWwindow* window, const double dt) {
	if (!hasFocus)
		return;
	
	glm::vec3 movement{};
	
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		movement += bodyForward;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		movement -= bodyRight;

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		movement -= bodyForward;

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		movement += bodyRight;

	if (glm::length(movement) > 0.0f) {
		movement = glm::normalize(movement);
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		movement += up;

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		movement -= up;

	const auto currentSpeed = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ? standardSpeed * 3.0f : standardSpeed;
	position += (float)dt * currentSpeed * movement;
}

void Camera::rotateCamera(GLFWwindow* window) {
	if (!hasFocus || glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		return;
	
	// THIS CAN BE MADE BETTER BUT ITS FINE.
	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);
	double halfWidth = (double)width / 2.0;
	double halfHeight = (double)height / 2.0;


	rotX += (mouseY - halfHeight) * sensitivity;
	rotY += (mouseX - halfWidth) * sensitivity;

	const auto ang = 89.9f;
	if (rotX > ang)
		rotX = ang;

	if (rotX < -ang)
		rotX = -ang;

	// UP DOWN ===
	eyesForward = glm::rotate(worldForward, glm::radians(rotX), glm::vec3{ 1.0f, 0.0f, 0.0f });

	// LEFT RIGHT ===
	eyesForward = glm::rotate(eyesForward, glm::radians(-rotY), up);
	bodyForward = glm::rotate(worldForward, glm::radians(-rotY), up);
	bodyRight = glm::rotate(bodyForward, glm::radians(-90.0f), up);

	// RESET ===
	glfwSetCursorPos(window, halfWidth, halfHeight);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
}

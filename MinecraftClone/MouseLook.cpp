#include "MouseLook.h"

MouseLook::MouseLook() = default;

MouseLook::MouseLook(GLFWwindow* window, const unsigned int width, const unsigned int height, const float sensitivity) :
	sensitivity{ sensitivity } {
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetCursorPos(window, (double)width / 2.0, (double)height / 2.0f);
	updateDirections();
}

void MouseLook::update(GLFWwindow* window, const unsigned int width, const unsigned int height, const bool hasFocus) {
	if (!hasFocus)
		return;

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

	updateDirections();

	// RESET. ===
	glfwSetCursorPos(window, halfWidth, halfHeight);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
}

void MouseLook::updateDirections() {
	// UP DOWN ===
	eyesForward = glm::rotate(worldForward, glm::radians(rotX), glm::vec3{ 1.0f, 0.0f, 0.0f });
	eyesUp = glm::rotate(worldUp, glm::radians(rotX), glm::vec3{ 1.0f, 0.0f, 0.0f });

	// LEFT RIGHT ===
	eyesForward = glm::rotate(eyesForward, glm::radians(-rotY), worldUp);
	eyesUp = glm::rotate(eyesUp, glm::radians(-rotY), worldUp);

	bodyForward = glm::rotate(worldForward, glm::radians(-rotY), worldUp);
	bodyRight = glm::rotate(bodyForward, glm::radians(-90.0f), worldUp);
}

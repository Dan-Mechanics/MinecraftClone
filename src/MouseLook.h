#pragma once
#include "minecraft_clone.h"
#include "Direction.h"

class MouseLook {
public:
	glm::vec3 eyesForward{};
	glm::vec3 eyesUp{};
	glm::vec3 bodyForward{};
	glm::vec3 bodyRight{};;

	float sensitivity{};
	float rotX{};
	float rotY{};

	MouseLook();
	MouseLook(GLFWwindow* window, const unsigned int width, const unsigned int height, const float sensitivity);

	void update(GLFWwindow* window, const unsigned int width, const unsigned int height, const bool hasFocus);

private:
	void updateDirections();

};


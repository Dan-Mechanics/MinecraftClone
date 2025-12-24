#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
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
	void rotateCamera(GLFWwindow* window, const unsigned int width, const unsigned int height, const bool hasFocus);
	void updateDirections();

};


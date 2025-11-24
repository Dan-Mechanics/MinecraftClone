#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "shader.h"

class Camera {
public:
	glm::vec3 position{};
	glm::vec3 eyesForward = glm::vec3{ 0.0f, 0.0f, -1.0f };
	glm::vec3 bodyForward = glm::vec3{ 0.0f, 0.0f, -1.0f };
	glm::vec3 up = glm::vec3{ 0.0f, 1.0f, 0.0f };
	glm::mat4 cameraMatrix = glm::identity<glm::mat4>();

	bool shouldCenterCursor{};
	const unsigned int width{};
	const unsigned int height{};
	const float standardSpeed{};
	const float sensitivity{};

	Camera();
	Camera(const unsigned int width, const unsigned int height, const float standardSpeed, const float sensitivity);

	void updateMatrix(float fovDeg, float nearPlane, float farPlane);
	void sendMatrixToShader(Shader& shader, const char* uniform) const;
	void moveCamera(GLFWwindow* window, const float tickInterval);
	void rotateCamera(GLFWwindow* window);

};

#endif 

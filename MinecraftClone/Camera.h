#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "Shader.h"
#include "Direction.h"

class Camera {
public:
	glm::vec3 position{};
	
	Camera();
	void sendMatrixToShader(const Shader& shader, const char* uniform) const;
	void updateMatrix(const float fovDeg, const float nearPlane, 
		const float farPlane, const glm::vec3& pos, const glm::vec3& forward,
		const unsigned int width, const unsigned int height);

private:
	glm::mat4 cameraMatrix{};

};
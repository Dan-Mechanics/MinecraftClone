#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "../Rendering/Shader.h"
#include "../World/Direction.h"
#include <vector>

class OrthographicCamera {
public:
	glm::vec3 position{};

	OrthographicCamera();
	void sendMatrixToShader(const Shader& shader, const char* uniform) const;
	void updateMatrix(const float left, const float right, const float bottom, const float top, const float zNear, const float zFar, const glm::vec3& pos, const glm::vec3& forward);

private:
	glm::mat4 matrix{};

};
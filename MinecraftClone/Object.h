#pragma once
#include "Mesh.h"

class Object {
public:
	glm::vec3 pos{};
	glm::vec3 rot{};
	glm::vec3 scale{};
	glm::vec4 color{};
	bool withTheClock = false;


	Object();
	Object(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale);
	void draw(const Mesh& mesh, const Shader& shader, const Camera& camera, glm::vec4 lightColor, glm::vec3 lightPos, glm::vec4 worldColor);
	void drawColor(const Mesh& mesh, const Shader& shader, const Camera& camera, glm::vec4 lightColor, glm::vec3 lightPos, glm::vec4 worldColor);
	void move(const glm::vec3& vel, const double dt);
	void rotate(const glm::vec3& vel, const double dt);
	void setColor(const glm::vec4& color);
	void setWithTheClock(bool value);
	void free() const;
};
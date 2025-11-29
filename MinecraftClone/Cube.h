#pragma once
#include "Mesh.h"

class Cube {
public:
	glm::vec3 pos{};
	glm::vec3 rot{};
	glm::vec3 scale{};
	glm::vec4 color{};
	Mesh mesh{};

	Cube();
	Cube(std::vector<Vertex>& verts, std::vector<GLuint>& tris, const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale);
	Cube(std::vector<Vertex>& verts, std::vector<GLuint>& tris, std::vector<Texture>& tex, const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale);
	void draw(const Shader& shader, const Camera& camera, glm::vec4 lightColor, glm::vec3 lightPos, glm::vec4 worldColor);
	void move(const glm::vec3& vel, const double dt);
	void setColor(const glm::vec4& color);
	//void setTexture(); // IS THIS ALLOWED EVEN ??
	void free() const;
};
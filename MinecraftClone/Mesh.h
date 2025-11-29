#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include <string>

#include "vao.h"
#include "ebo.h"
#include "Camera.h"
#include "Texture.h"
#include <vector>

class Mesh {
public:
	std::vector<Vertex> vertices{};
	std::vector<GLuint> indices{};
	std::vector<Texture> textures{};
	vao vao{};
	ebo ebo{};
	vbo vbo{};

	Mesh();
	Mesh(std::vector<Vertex>& verticies, std::vector<GLuint>& indices, std::vector<Texture>& textures);

	void draw(const Shader& shader, const Camera& camera, glm::mat4 matrix, glm::vec3 translation,
		glm::quat rotation, glm::vec3 scale, glm::vec3 lightPos, glm::vec4 lightColor, glm::vec4 worldColor) const;

	void drawColor(const Shader& shader, const Camera& camera, glm::mat4 matrix, glm::vec3 translation,
		glm::quat rotation, glm::vec3 scale, glm::vec3 lightPos, glm::vec4 lightColor, glm::vec4 worldColor, glm::vec4 selfColor) const;

	void free() const;


};

#endif
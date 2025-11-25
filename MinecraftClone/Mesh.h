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
	void draw(Shader& shader, Camera& camera, glm::mat4 matrix, glm::vec3 translation, glm::quat rotation, glm::vec3 scale);
	void free() const;

};

#endif
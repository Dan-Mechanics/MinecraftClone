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
	glm::mat4 modelMatrix{};
	vao vao{};
	ebo ebo{};
	vbo vbo{};

	Mesh();
	Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices);
	Mesh(const std::vector<Vertex>& verticies, const std::vector<GLuint>& indices, const glm::mat4& modelMatrix);

	void drawTexture(const Shader& shader, const Camera& camera,
		const glm::vec3& translation, const glm::quat& rotation,
		const glm::vec3& scale, const glm::vec3& lightPos,
		const glm::vec4& lightColor, const glm::vec4& worldColor,
		const std::vector<Texture>& textures) const;

	void drawColor(const Shader& shader, const Camera& camera,
		const glm::vec3& translation, const glm::quat& rotation,
		const glm::vec3& scale, const glm::vec3& lightPos,
		const glm::vec4& lightColor, const glm::vec4& worldColor,
		const glm::vec4& selfColor) const;

	void drawUnlit(const Shader& shader, const Camera& camera,
		const glm::vec3& translation, const glm::quat& rotation,
		const glm::vec3& scale, const glm::vec4& selfColor) const;

	void free() const;

};

#endif
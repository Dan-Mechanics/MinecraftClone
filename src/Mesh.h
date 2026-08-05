#pragma once
#include "minecraft_clone.h"
#include "vao.h"
#include "ebo.h"
#include "Camera.h"
#include "Texture.h"

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
	Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const glm::mat4& modelMatrix);

	void drawTexture(const Shader& shader, const Camera& camera,
		const glm::vec3& translation, const glm::quat& rotation,
		const glm::vec3& scale, const glm::vec3& lightPos,
		const glm::vec4& lightColor, const glm::vec4& worldColor, const std::vector<Texture>& material) const;

	void drawUnlitTexture(const Shader& shader, const Camera& camera,
		const glm::vec3& translation, const glm::quat& rotation,
		const glm::vec3& scale, const std::vector<Texture>& material) const;

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
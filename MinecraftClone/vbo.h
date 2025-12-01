#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <vector>

struct Vertex {
public:
	glm::vec3 position{};
	glm::vec3 normal{};
	glm::vec3 color{};
	glm::vec2 texUv{};

	Vertex(const glm::vec3& position)
		: position(position) {
	}

	Vertex(const glm::vec3& position, const glm::vec3& normal)
		: position(position), normal(normal) { }

	Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec3& color, const glm::vec2& texUv)
		: position(position), normal(normal), color(color), texUv(texUv) { }

	Vertex() = default;
};

class vbo {
public:
	GLuint id{};
	vbo();
	vbo(const std::vector<Vertex>& vertices);

	void bind() const;
	void unbind() const;
	void free() const;
};

#endif
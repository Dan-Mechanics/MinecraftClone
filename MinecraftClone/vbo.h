#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <vector>

struct Vertex {
public:
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 texUv;

};

class vbo {
public:
	GLuint id;
	vbo(std::vector<Vertex>& vertices);

	void bind() const;
	void unbind() const;
	void free() const;
};

#endif
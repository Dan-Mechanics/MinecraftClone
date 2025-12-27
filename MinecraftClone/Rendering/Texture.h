#pragma once
#include <glad/glad.h>
#include "Shader.h"
#include <stdexcept>

class Texture {
public:
	GLuint id;

	// THIS SEEMS STRANGE...
	const char* type;
	GLuint unit;

	Texture(const std::string& image, const std::string& type, GLuint slot);

	void setTextureUnit(const Shader & shader, const char* uniform, GLuint unit) const;
	void bind() const;
	void unbind() const;
	void free() const;

};
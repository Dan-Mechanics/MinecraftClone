#pragma once
#include <glad/glad.h>
#include <stb/stb_image.h>
#include "shader.h"

class Texture {
public:
	GLuint id;
	const char* type;
	GLuint unit;

	Texture(const char* image, const char* texType, GLuint slot);

	void setTextureUnit(const Shader & shader, const char* uniform, GLuint unit) const;
	void bind() const;
	void unbind() const;
	void free() const;

};
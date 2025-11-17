#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <glad/glad.h>
#include <stb/stb_image.h>

#include "shaderClass.h"

class Texture{
public:
	GLuint id;
	const char* type;
	GLuint unit;

	Texture(const char* image, const char* texType, GLuint slot, GLenum format, GLenum pixelType);

	void setTextureUnit(Shader& shader, const char* uniform, GLuint unit) const;
	void bind() const;
	void unbind() const;
	void free() const;
};

#endif 
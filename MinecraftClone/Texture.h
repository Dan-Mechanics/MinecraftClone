#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <glad/glad.h>
#include <stb/stb_image.h>

#include "shaderClass.h"

class Texture{
public:
	GLuint id;
	GLenum type;
	Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);

	void setTextureUnit(Shader shader, const char* uniform, GLuint unit) const;
	void bind() const;
	void unbind() const;
	void free() const;
};

#endif 
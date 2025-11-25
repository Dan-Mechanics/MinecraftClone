#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <glad/glad.h>
#include <stb/stb_image.h>
#include "shader.h"

class Texture {
public:
	GLuint id;
	const char* type;
	GLuint unit;

	Texture(const char* image, const char* texType, GLuint slot);

	// Assigns a texture unit to a texture
	void setTextureUnit(const Shader & shader, const char* uniform, GLuint unit) const;
	// Binds a texture
	void bind() const;
	// Unbinds a texture
	void unbind() const;
	// Deletes a texture
	void free() const;

};

#endif
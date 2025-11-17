#include "Texture.h"

Texture::Texture(const char* image, GLenum texType, GLuint slot, GLenum format, GLenum pixelType) {
	type = texType;
	int widthImg, heightImg, numColorChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColorChannels, 0);

	glGenTextures(1, &id);
	glActiveTexture(GL_TEXTURE0 + slot);
	unit = slot;
	glBindTexture(texType, id);

	glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(texType, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, pixelType, bytes);
	glGenerateMipmap(texType);

	stbi_image_free(bytes);
	glBindTexture(texType, 0);
}

void Texture::setTextureUnit(Shader& shader, const char* uniform, GLuint unit) const {
	GLuint tex0uni = glGetUniformLocation(shader.id, uniform);
	shader.activate();
	glUniform1f(tex0uni, unit);
}

void Texture::bind() const {
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(type, id);
}

void Texture::unbind() const {
	glBindTexture(type, 0);
}

void Texture::free() const {
	glDeleteTextures(1, &id);
}

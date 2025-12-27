#pragma once
#include <glad/glad.h>
#include "../Core/file_utils.h"

class Shader {
public:
	GLuint id{};
	Shader();
	Shader(const char* vertexFile, const char* fragementFile);

	void activate() const;
	void free() const;

private:
	void compileErrors(unsigned int shader, const char* type);

};
#pragma once
#include "minecraft_clone.h"
#include "file_utils.h"

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
#pragma once
#include "minecraft_clone.h"

#define SHADER_PATH "res/shaders/"
#define TEXTURE_PATH "res/textures/"

std::string getFileContent(const std::string& path);
std::string getShaderCode(const std::string& shaderName);
std::string getTexturePath(const std::string& textureName);
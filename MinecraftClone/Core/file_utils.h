#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

#define SHADER_PATH "Resources/Shaders/"
#define TEXTURE_PATH "Resources/Textures/"

std::string getFileContent(const std::string& path);
std::string getShaderCode(const std::string& shaderName);
std::string getTexturePath(const std::string& textureName);
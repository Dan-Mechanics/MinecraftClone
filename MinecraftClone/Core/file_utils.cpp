#include "file_utils.h"

std::string getFileContent(const std::string& path) {
	std::ifstream file{ path };
	if (!file.is_open()) 
		throw(errno);

	std::ostringstream filecontent;
	filecontent << file.rdbuf();
	return filecontent.str();
}

std::string getShaderCode(const std::string& shaderName) {
	return getFileContent(SHADER_PATH + shaderName);
}

std::string getTexturePath(const std::string& textureName) {
	return TEXTURE_PATH + textureName;
}

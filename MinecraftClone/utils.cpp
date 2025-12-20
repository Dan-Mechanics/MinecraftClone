#include "utils.h"

int randomInclusive(const int min, const int max) {
	return rand() % (max - min + 1) + min;
}

void logVec3(const glm::vec3& vec) {
	std::cout << vec.x << " " << vec.y << " " << vec.z << std::endl;
}

void logIvec3(const glm::ivec3& vec) {
	std::cout << vec.x << " " << vec.y << " " << vec.z << std::endl;
}

std::string ivec3ToString(const glm::ivec3& vec) {
	const auto space = " ";
	return std::to_string(vec.x) + space + std::to_string(vec.y) + space + std::to_string(vec.z);
}

void freeMaterial(const std::vector<Texture>& material) {
	auto it = material.begin();
	while (it != material.end()) {
		it->free();
		++it;
	}
}

void bindMaterial(std::vector<Texture>& material, const Shader& shader) {
	auto diffuseCount = 0;
	auto specularCount = 0;
	for (int i = 0; i < material.size(); ++i) {
		std::string num;
		std::string type = material[i].type;
		if (type == "diffuse") {
			num = std::to_string(diffuseCount++);
		}
		else if (type == "specular") {
			num = std::to_string(specularCount++);
		}
		// POSSIBLY ADD EMMISION IN THE FUTURE ??

		material[i].setTextureUnit(shader, (type + num).c_str(), i);
		material[i].bind();
	}
}
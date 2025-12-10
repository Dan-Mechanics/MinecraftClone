#include "utils.h"

int randomInclusive(const int min, const int max) {
	return rand() % (max - min + 1) + min;
}

void log(const glm::vec3& vec) {
	std::cout << vec.x << " " << vec.y << " " << vec.z << std::endl;
}

void log(const glm::ivec3& vec) {
	std::cout << vec.x << " " << vec.y << " " << vec.z << std::endl;
}

void freeMaterial(const std::vector<Texture>& material) {
	auto it = material.begin();
	while (it != material.end()) {
		it->free();
		++it;
	}
}
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

Direction getDirectionFromVector(const glm::vec3& vec) {
	int axis = 0;
	float highest = abs(vec.x);

	if (abs(vec.y) > highest) 
		axis = 1;

	if (abs(vec.z) > highest) 
		axis = 2;

	switch (axis) {
	case 0:
		return vec.x > 0.0f ? Direction::RIGHT : Direction::LEFT;
	case 1:
		return vec.y > 0.0f ? Direction::UP : Direction::DOWN;
	case 2:
		return vec.z > 0.0f ? Direction::FORWARD : Direction::BACK;
	default:
		return Direction::UP;
	}
}

glm::ivec3 directionToIvec3(const Direction dir) {
	switch (dir) {
	case UP:
		return up;
	case DOWN:
		return down;
	case FORWARD:
		return forward;
	case RIGHT:
		return right;
	case BACK:
		return back;
	case LEFT:
		return left;
	default:
		return up;
	}
}

bool validPositionsToBlockPos(const glm::vec3& a, const glm::vec3& b, glm::ivec3& blockPos) {
	const auto difference = glm::abs(a - b);

	// ! ??
	if (difference.x > 1.001f || difference.y > 1.001f || difference.z > 1.001f)
		return false;

	blockPos = glm::floor((a + b) / 2.0f);
	return true;
}

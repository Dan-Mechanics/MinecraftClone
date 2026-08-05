#pragma once
#include "../World/World.h"

class Terraformer {
public:
	Terraformer();

	void update(GLFWwindow* window, const Raycast& raycast, World& world, ThreadPool& pool, const Atlas& atlas, const glm::ivec3& bannedBlockPos);
	bool getFaceHighlight(const Raycast& raycast, const World& world, glm::vec3& pos, glm::vec3& scale);
	void setBlockType(const BlockType blockType);

private:
	bool prevLeftPressed{};
	bool prevRightPressed{};
	BlockType blockType{};
	int ticks{};
	bool visible{};

	void remove(const Raycast& raycast, World& world, ThreadPool& pool, const Atlas& atlas) const;
	void add(const Raycast& raycast, World& world, ThreadPool& pool, const Atlas& atlas, const glm::ivec3 & eyeBlockPos) const;

};
#pragma once
#include "World.h"

class Terraformer {
public:
	Terraformer();
	Terraformer(const float range);

	void update(GLFWwindow* window, const Camera& camera, World& world, ThreadPool& pool, const Atlas& atlas);
	void setBlockType(const BlockType blockType);

private:
	float range{};
	bool prevLeftPressed{};
	bool prevRightPressed{};
	BlockType blockType{};

	void remove(const Camera& camera, World& world, ThreadPool& pool, const Atlas& atlas) const;
	void add(const Camera& camera, World& world, ThreadPool& pool, const Atlas& atlas) const;
};
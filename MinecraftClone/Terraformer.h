#pragma once
#include "World.h"

class Terraformer {
public:
	Terraformer();

	void update(GLFWwindow* window, const Raycast& raycast, World& world, ThreadPool& pool, const Atlas& atlas);
	void setBlockType(const BlockType blockType);

private:
	bool prevLeftPressed{};
	bool prevRightPressed{};
	BlockType blockType{};

	void remove(const Raycast& raycast, World& world, ThreadPool& pool, const Atlas& atlas) const;
	void add(const Raycast& raycast, World& world, ThreadPool& pool, const Atlas& atlas) const;
};
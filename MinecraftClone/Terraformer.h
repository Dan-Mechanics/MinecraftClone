#pragma once
#include "World.h"

class Terraformer {
public:
	Terraformer();

	void update(GLFWwindow* window, const Camera& camera, World & world, ThreadPool& pool, const Atlas& atlas);

private:
	bool prevLeftPressed{};
	bool prevRightPressed{};

	void remove(const Camera& camera, World& world, ThreadPool& pool, const Atlas& atlas);
	void add(const Camera& camera, World& world, ThreadPool& pool, const Atlas& atlas);
};
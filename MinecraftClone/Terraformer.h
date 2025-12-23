#pragma once
#include "World.h"

class Terraformer {
public:
	Terraformer();
	Terraformer(const float range);

	void update(GLFWwindow* window, const Camera& camera, World & world, ThreadPool& pool, const Atlas& atlas);

private:
	float range{};
	bool prevLeftPressed{};
	bool prevRightPressed{};

	void remove(const Camera& camera, World& world, ThreadPool& pool, const Atlas& atlas) const;
	void add(const Camera& camera, World& world, ThreadPool& pool, const Atlas& atlas) const;
};
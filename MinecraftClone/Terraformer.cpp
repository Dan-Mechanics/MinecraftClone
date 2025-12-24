#include "Terraformer.h"

Terraformer::Terraformer() = default;
Terraformer::Terraformer(const float range) : range{ range } { }

void Terraformer::update(GLFWwindow* window, const Camera& camera, World & world, ThreadPool& pool, const Atlas& atlas) {
	const auto leftPressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
	const auto rightPressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;

	if (leftPressed && !prevLeftPressed)
		remove(camera, world, pool, atlas);
	
	if (rightPressed && !prevRightPressed)
		add(camera, world, pool, atlas);

	prevLeftPressed = leftPressed;
	prevRightPressed = rightPressed;
}

void Terraformer::setBlockType(const BlockType blockType) {
	this->blockType = blockType;
}

void Terraformer::remove(const Camera& camera, World& world, ThreadPool& pool, const Atlas& atlas) const {
	glm::ivec3 blockPos{};
	glm::ivec3 normal{};
	if (!world.raycast(camera.position, camera.eyesForward, range, blockPos, normal))
		return;

	world.remove(blockPos);
	world.reloadSingleChunkMesh(pool, atlas);
}

void Terraformer::add(const Camera& camera, World& world, ThreadPool& pool, const Atlas& atlas) const {
	glm::ivec3 blockPos{};
	glm::ivec3 normal{};
	if (!world.raycast(camera.position, camera.eyesForward, range, blockPos, normal))
		return;

	world.add(blockPos + normal, blockType);
	world.reloadSingleChunkMesh(pool, atlas);
}

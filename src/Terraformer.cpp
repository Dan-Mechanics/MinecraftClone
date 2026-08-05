#include "Terraformer.h"

Terraformer::Terraformer() = default;

void Terraformer::update(GLFWwindow* window, const Raycast& raycast, World & world, ThreadPool& pool, const Atlas& atlas, const glm::ivec3& bannedBlockPos) {
	const auto leftPressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
	const auto rightPressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;

	if (leftPressed && !prevLeftPressed)
		remove(raycast, world, pool, atlas);
	
	if (rightPressed && !prevRightPressed)
		add(raycast, world, pool, atlas, bannedBlockPos);

	prevLeftPressed = leftPressed;
	prevRightPressed = rightPressed;
}

bool Terraformer::getFaceHighlight(const Raycast& raycast, const World& world, glm::vec3& pos, glm::vec3& scale) {
	ticks++;
	if (ticks >= 8) {
		ticks = 0;
		visible = !visible;
	}
	
	if (!visible)
		return false;

	glm::ivec3 blockPos{};
	glm::ivec3 normal{};
	if (!world.raycast(raycast, blockPos, normal))
		return false;

	const auto bump = 0.1f;
	glm::vec3 absNormal = glm::abs(normal);
	scale = glm::vec3{ 1.0f } - absNormal + absNormal * bump;
	pos = glm::vec3{ blockPos + normal } + glm::vec3{ 0.5f } - glm::vec3{ normal } * 0.5f;
	return true;
}

void Terraformer::setBlockType(const BlockType blockType) {
	this->blockType = blockType;
}

void Terraformer::remove(const Raycast& raycast, World& world, ThreadPool& pool, const Atlas& atlas) const {
	glm::ivec3 blockPos{};
	glm::ivec3 normal{};
	if (!world.raycast(raycast, blockPos, normal))
		return;

	world.remove(blockPos);
	const auto chunkPos = blockPosToChunkPos(blockPos, world.getChunkSize());
	world.reloadChunkAtPos(chunkPos, atlas);
}

void Terraformer::add(const Raycast& raycast, World& world, ThreadPool& pool,
	const Atlas& atlas, const glm::ivec3& eyeBlockPos) const {
	glm::ivec3 blockPos{};
	glm::ivec3 normal{};
	if (!world.raycast(raycast, blockPos, normal))
		return;

	blockPos += normal;
	if (blockPos == eyeBlockPos || blockPos == eyeBlockPos + down)
		return;

	world.add(blockPos, blockType);
	const auto chunkPos = blockPosToChunkPos(blockPos, world.getChunkSize());
	world.reloadChunkAtPos(chunkPos, atlas);
}

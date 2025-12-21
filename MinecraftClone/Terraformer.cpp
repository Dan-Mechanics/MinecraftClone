#include "Terraformer.h"

Terraformer::Terraformer() = default;

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

void Terraformer::remove(const Camera& camera, World& world, ThreadPool& pool, const Atlas& atlas) {
	glm::ivec3 hitBlock;
	if (!world.raycast(camera.position, camera.eyesForward, 100, 0.1f, hitBlock))
		return;

	world.remove(hitBlock);
	world.flush(pool, atlas);
}

void Terraformer::add(const Camera& camera, World& world, ThreadPool& pool, const Atlas& atlas) {
	glm::vec3 point;
	glm::ivec3 hitBlock;
	if (!world.exactRaycast(camera.position, camera.eyesForward, 100, 0.1f, hitBlock, point))
		return;

	glm::vec3 direction = point - glm::vec3{ hitBlock };
	Direction normal = getDirectionFromVector(direction);
	hitBlock += directionToIvec3(normal);

	world.add(hitBlock, BlockType::SAPPHIRE);
	world.flush(pool, atlas);
}

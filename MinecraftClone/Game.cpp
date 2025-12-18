#include "Game.h"

Game::Game() = default;

void Game::setup(GLFWwindow* window, const unsigned int width, const unsigned int height) {
	sunColor = glm::vec4{
		(float)255 / 255,
		(float)255 / 255,
		(float)255 / 255, 1.0f
	};

	ambientColor = glm::vec4{
		(float)90 / 255,
		(float)86 / 255,
		(float)150 / 255, 1.0f
	};

	// ===

	sun.setPos(glm::vec3{ 0.5f, 0.4f, 0.5f } *20.0f);
	sun.setColor(sunColor);

	centerLine.setScale(glm::vec3{ 0.2f, 100.0f, 0.02f });
	centerLine.setColor(glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });

	center.setScale(glm::vec3{ 0.5f });
	center.setColor(glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });

	forward.setPos(glm::vec3{ 0.0f, 0.0f, 3.0f });
	forward.setColor(glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f });

	right.setPos(glm::vec3{ 3.0f, 0.0f, 0.0f });
	right.setColor(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });

	up.setPos(glm::vec3{ 0.0f, 3.0f, 0.0f });
	up.setColor(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });

	ground.setPos(glm::vec3{ 0.0f, -3.0f, 0.0f });
	ground.setScale(glm::vec3{ 100.0f, 1.0f, 100.0f });

	// ===

	std::vector<Vertex> cubeVerts{};
	std::vector<GLuint> cubeTris{};
	glm::mat4 cubeMatrix;

	getCubeMesh(cubeVerts, cubeTris, cubeMatrix);
	cubeMesh = { cubeVerts, cubeTris, cubeMatrix };

	// ===

	float standardSpeed = 15.0f;
	float sensitivity = 0.1f;
	camera = { window, width, height, standardSpeed, sensitivity };

	// ===

	atlas = generateAtlas();
	chunkSize = 16;
	renderRadius = 5;
	updateVisibleAreaInterval = 1.0f;
	world = { chunkSize, renderRadius };

	// ===

	materialShader = { "default.vert", "material.frag" };
	unlitShader = { "default.vert", "unlit_color.frag" };
	shadowMapShader = { "shadow_map.vert", "shadow_map.frag" };
	chunkShader = { "chunk.vert", "chunk.frag" };
	chunkShadowMap = { "chunk_shadow.vert", "shadow_map.frag" };
	
	shadowMap = { 2048, 2048, 35.0f };

	// ===

	woodMaterial = {
		Texture("planks.png", "diffuse", 0),
		Texture("planks_specular.png", "specular", 1)
	};

	// YOU DON'T HAVE TO INCLUDE SPECULAR.
	atlasMaterial = {
		Texture("atlas.png", "diffuse", 0)
	};
}

void Game::draw(const float deltaTime, const bool hasFocus, GLFWwindow* window) {
	centerLine.drawAsUnlitColor(cubeMesh, unlitShader, camera);
	forward.drawAsUnlitColor(cubeMesh, unlitShader, camera);
	center.drawAsUnlitColor(cubeMesh, unlitShader, camera);
	right.drawAsUnlitColor(cubeMesh, unlitShader, camera);
	up.drawAsUnlitColor(cubeMesh, unlitShader, camera);

	bindMaterial(atlasMaterial, chunkShader);
	world.draw(atlasMaterial, chunkShader, camera, sun.color, sun.pos, ambientColor);
}

void Game::drawShadows(const float deltaTime, const bool hasFocus, GLFWwindow* window) {
	camera.hasFocus = hasFocus;
	camera.moveCamera(window, deltaTime);
	camera.rotateCamera(window);
	camera.updateMatrix(105.0f, 0.01f, 100.0f);

	// ===
	
	shadowMap.bind(camera, sun, chunkShadowMap);
	world.drawShadows(chunkShadowMap, camera);
	shadowMap.sendToShader(chunkShader);
}

void Game::tick(const float interval, ThreadPool& pool) {
	timer += interval;
	if (timer >= updateVisibleAreaInterval) {
		timer = 0.0f;
		world.tick(pool, camera.position);
		return;
	}

	// THIS THROTTLES THE WORKLOAD OVER MORE FRAMES.
	const int reloadCount = 2;
	for (int i = 0; i < reloadCount; ++i) {
		world.reloadSingleChunkMesh(pool, atlas);
	}
}

glm::vec4 Game::getClearColor() const {
	return ambientColor;
}

void Game::free() {
	materialShader.free();
	unlitShader.free();

	world.free();

	freeMaterial(woodMaterial);
	freeMaterial(atlasMaterial);

	cubeMesh.free();
	shadowMap.free();
}

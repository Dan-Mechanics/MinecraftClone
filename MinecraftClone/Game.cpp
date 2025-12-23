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
		(float)110 / 255,
		(float)194 / 255, 1.0f
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

	crosshair.setPos(glm::vec3{ 0.0f, 0.0f, 3.0f });
	crosshair.setColor(glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });
	crosshair.setScale(glm::vec3{ 0.05f });

	held.setScale(glm::vec3{ 0.8f });

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
	displayCamera = camera;
	displayCamera.updateDirections();

	// ===

	atlas = generateAtlas();
	const auto chunkSize = 16;
	const auto maxRenderDistance = 3;
	worldTickInterval = 0.5f;
	world = { chunkSize, maxRenderDistance };

	terraformer = { 10.0f };

	// ===

	materialShader = { "default.vert", "material.frag" };
	unlitShader = { "default.vert", "unlit_color.frag" };
	shadowMapShader = { "shadow_map.vert", "shadow_map.frag" };
	chunkMaterialShader = { "chunk.vert", "chunk.frag" };
	chunkShadowMapShader = { "chunk_shadow.vert", "shadow_map.frag" };
	
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

void Game::update(const float deltaTime, const bool hasFocus, GLFWwindow* window, ThreadPool& pool) {
	camera.hasFocus = hasFocus;
	camera.moveCamera(window, deltaTime);
	camera.rotateCamera(window);

	camera.updateMatrix(105.0f, 0.01f, 100.0f);
	displayCamera.updateMatrix(105.0f, 0.01f, 100.0f);

	terraformer.update(window, camera, world, pool, atlas);
}

void Game::draw(const float deltaTime, const bool hasFocus, GLFWwindow* window) {
	centerLine.drawAsUnlitColor(cubeMesh, unlitShader, camera);
	forward.drawAsUnlitColor(cubeMesh, unlitShader, camera);
	center.drawAsUnlitColor(cubeMesh, unlitShader, camera);
	right.drawAsUnlitColor(cubeMesh, unlitShader, camera);
	up.drawAsUnlitColor(cubeMesh, unlitShader, camera);
	 
	// MAKE IT SO WE DON'T HAVE TO BIND
	// A TEXTURE FOR EACH SEPARATE CHUNK.
	bindMaterial(atlasMaterial, chunkMaterialShader);
	world.draw(atlasMaterial, chunkMaterialShader, camera, sun.color, sun.pos, ambientColor);
}

void Game::drawDisplay(const float deltaTime, const bool hasFocus, GLFWwindow* window) {
	held.setPos(camera.position + camera.eyesForward + camera.bodyRight - camera.eyesUp);
	held.setRot(glm::vec3{ camera.rotX, -camera.rotY, 0.0f });
	held.drawWithMaterial(cubeMesh, atlasMaterial, materialShader, camera, sun.color, sun.pos, ambientColor);

	crosshair.drawAsUnlitColor(cubeMesh, unlitShader, displayCamera);
}

void Game::drawShadows(const float deltaTime, const bool hasFocus, GLFWwindow* window) {
	shadowMap.activate(camera, sun);
	shadowMap.bind(chunkShadowMapShader);

	world.drawShadows(chunkShadowMapShader, camera);
	shadowMap.sendToShader(chunkMaterialShader);
}

void Game::tick(const float interval, ThreadPool& pool, GLFWwindow* window) {
	timer += interval;
	if (timer >= worldTickInterval) {
		timer = 0.0f;

		if (world.toggle) {
			world.allocateNewChunks(pool, camera.position);
		}
		else {
			world.destroyOldChunks(pool, camera.position);
		}

		world.toggle = !world.toggle;
		return;
	}

	world.reloadSingleChunkMesh(pool, atlas);
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

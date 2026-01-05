#include "Game.h"

Game::Game() = default;
Game::Game(const unsigned int width, const unsigned int height)
	: width{ width }, height{ height } { }

void Game::setup(GLFWwindow* window) {
	sunColor = glm::vec4 {
		(float)255 / 255,
		(float)255 / 255,
		(float)255 / 255, 1.0f
	};

	ambientColor = glm::vec4 {
		(float)90 / 255,
		(float)110 / 255,
		(float)194 / 255, 1.0f
	};

	/*ambientColor = glm::vec4 {
		(float)100 / 255,
		(float)145 / 255,
		(float)190 / 255, 1.0f
	};*/

	// ===

	sun.setPos(glm::vec3{ 0.5f, 1.125f, 0.65f } * 20.0f);
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

	faceHighlight.setColor({ 1.0f, 1.0f, 1.0f, 1.0f });

	hotbarRotation = -10.0f;
	slot.setScale(glm::vec3{ 0.375f });
	heldBlock.setPos (
		worldForward * 1.25f +
		worldLeft - worldUp
	);

	// ===

	std::vector<Vertex> cubeVerts{};
	std::vector<GLuint> cubeTris{};
	glm::mat4 cubeMatrix;

	getCubeMesh(cubeVerts, cubeTris, cubeMatrix);
	cubeMesh = { cubeVerts, cubeTris, cubeMatrix };

	// ===

	const glm::vec3 playerSpawnPos{ 0.0f, 30.0f, 0.0f };
	playerMovement = { 10.0f, playerSpawnPos };
	mouseLook = { window, width, height, 0.1f };

	// ===

	atlas = generateAtlas();
	const auto chunkSize = 16;
	const auto renderDistance = 3;
	world = { chunkSize, renderDistance };

	for (int i = 0; i < BlockType::END; ++i) {
		setCubeFacesAsBlockType(cubeVerts, atlas, static_cast<BlockType>(i));
		singleBlockMeshes.emplace_back(cubeVerts, cubeTris, cubeMatrix);
	}

	// ===

	materialShader = { "default.vert", "material.frag" };
	simpleMaterialShader = { "default.vert", "simple_material.frag" };
	unlitShader = { "default.vert", "unlit.frag" };
	shadowMapShader = { "shadow_map.vert", "shadow_map.frag" };
	chunkShader = { "chunk.vert", "chunk.frag" };
	translucentChunkShader = { "chunk.vert", "translucent_chunk.frag" };
	chunkShadowShader = { "chunk_shadow.vert", "shadow_map.frag" };
	
	shadowMap = { 2048, 2048, 50.0f };

	// ===

	woodMaterial = {
		Texture(getTexturePath("planks.png"), "diffuse", 0),
		Texture(getTexturePath("planks_specular.png"), "specular", 1)
	};

	// YOU DON'T HAVE TO INCLUDE SPECULAR.
	atlasMaterial = {
		Texture(getTexturePath("atlas.png"), "diffuse", 0)
	};
}

void Game::update(const float deltaTime, const bool hasFocus, int& scrollInput, GLFWwindow* window, ThreadPool& pool) {
	mouseLook.update(window, width, height, hasFocus);
	playerMovement.move(window, mouseLook.bodyRight, mouseLook.bodyForward, deltaTime);
	playerMovement.collideWithWorld(world.getChunks(), world.getChunkSize());

	camera.updateMatrix(105.0f, 0.01f, 100.0f, playerMovement.pos, mouseLook.eyesForward, width, height);
	uiCamera.updateMatrix(105.0f, 0.01f, 100.0f, worldOrigin, worldForward, width, height);

	// ===

	world.update(deltaTime, atlas, pool, playerMovement.pos);

	const auto hasScrolled = blockSelector.onScroll(scrollInput);
	if (hasScrolled) 
		terraformer.setBlockType(blockSelector.getBlockType());

	const auto raycast = Raycast{ playerMovement.pos, mouseLook.eyesForward, 10.0f };
	terraformer.update(window, raycast, world, pool, atlas, posToBlockPos(playerMovement.pos));
}

void Game::draw() {
	centerLine.drawAsUnlitColor(cubeMesh, unlitShader, camera);
	forward.drawAsUnlitColor(cubeMesh, unlitShader, camera);
	center.drawAsUnlitColor(cubeMesh, unlitShader, camera);
	right.drawAsUnlitColor(cubeMesh, unlitShader, camera);
	up.drawAsUnlitColor(cubeMesh, unlitShader, camera);
	faceHighlight.drawAsUnlitColor(cubeMesh, unlitShader, camera);

	// MAKE IT SO WE DON'T HAVE TO BIND
	// A TEXTURE FOR EACH SEPARATE CHUNK.
	bindMaterial(atlasMaterial, chunkShader);
	world.draw(atlasMaterial, chunkShader, camera, sun.color, sun.pos, ambientColor);
}

void Game::drawUI(const float deltaTime, const bool hasFocus, GLFWwindow* window) {
	slot.setPos(heldBlock.pos);

	slot.pos -= worldLeft;
	slot.rotateOverTime(worldUp * hotbarRotation, deltaTime);

	for (int i = 0; i < 3; ++i) {
		auto index = blockSelector.getBlockType() + i + 1;
		if (index < 0)
			index += BlockType::END;

		if (index >= BlockType::END)
			index -= BlockType::END;

		slot.drawWithMaterial(singleBlockMeshes[index], atlasMaterial, simpleMaterialShader, uiCamera, sun.color, sun.pos, ambientColor);
		slot.pos -= worldLeft;
	}

	heldBlock.drawWithMaterial(singleBlockMeshes[blockSelector.getBlockType()],
		atlasMaterial, simpleMaterialShader, uiCamera, sun.color, sun.pos, ambientColor);

	crosshair.drawAsUnlitColor(cubeMesh, unlitShader, uiCamera);
}

void Game::drawShadows(const float deltaTime, const bool hasFocus, GLFWwindow* window) {
	shadowMap.activate(camera, sun);
	shadowMap.bind(chunkShadowShader);

	world.drawShadows(chunkShadowShader, camera);
	shadowMap.sendToShader(chunkShader);
	shadowMap.sendToShader(translucentChunkShader);
}

void Game::drawTranslucent() {
	world.drawTranslucent(atlasMaterial, translucentChunkShader, camera, sun.color, sun.pos, ambientColor);
}

void Game::tick(const float interval, ThreadPool& pool, GLFWwindow* window) {
	const auto raycast = Raycast{ playerMovement.pos, mouseLook.eyesForward, 10.0f };

	glm::vec3 pos{};
	glm::vec3 scale{};
	faceHighlight.setVisible(terraformer.getFaceHighlight(raycast, world, pos, scale));

	if (faceHighlight.visible) {
		faceHighlight.setPos(pos);
		faceHighlight.setScale(scale);
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

	auto it = singleBlockMeshes.begin();
	while (it != singleBlockMeshes.end()) {
		it->free();
		++it;
	}

	cubeMesh.free();
	shadowMap.free();
}

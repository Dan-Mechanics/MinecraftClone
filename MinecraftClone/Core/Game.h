#pragma once
#include <glm/ext/vector_float4.hpp>
#include <glm/ext/vector_float3.hpp>
#include "Object.h"
#include "../Rendering/mesh_utils.h"
#include "../World/World.h"
#include "../Rendering/ShadowMapFBO.h"
#include "../Player/Terraformer.h"
#include "../Player/BlockSelector.h"
#include "../Player/MouseLook.h"
#include "../Player/PlayerMovement.h"

class Game {
public:

	Game();
	Game(const unsigned int width, const unsigned int height);

	void setup(GLFWwindow* window);
	void update(const float deltaTime, const bool hasFocus, int& scrollInput, GLFWwindow* window, ThreadPool& pool);
	void draw();
	void drawUI(const float deltaTime, const bool hasFocus, GLFWwindow* window);
	void drawShadows(const float deltaTime, const bool hasFocus, GLFWwindow* window);
	void drawTranslucent();
	void tick(const float interval, ThreadPool& pool, GLFWwindow* window);

	glm::vec4 getClearColor() const;
	void free();

private:
	unsigned int width{};
	unsigned int height{};
	float hotbarRotation{};

	glm::vec4 ambientColor{};
	glm::vec4 sunColor{};

	Object sun{};
	Object centerLine{};
	Object center{};
	Object forward{};
	Object right{};
	Object up{};
	Object ground{};

	Object faceHighlight{};
	Object waterScreen{};

	Object crosshair{};
	Object heldBlock{};
	Object slot{};

	World world{};
	Atlas atlas{};

	Camera camera{};
	Camera uiCamera{};

	MouseLook mouseLook{};
	PlayerMovement playerMovement{};

	Terraformer terraformer{};
	BlockSelector blockSelector{};

	Shader materialShader{};
	Shader simpleMaterialShader{};
	Shader unlitShader{};

	Shader chunkShader{};
	Shader translucentChunkShader{};
	Shader chunkShadowShader{};

	Shader shadowMapShader{};
	ShadowMapFBO shadowMap{};

	std::vector<Texture> woodMaterial{};
	std::vector<Texture> atlasMaterial{};

	std::vector<Mesh> singleBlockMeshes{};
	Mesh cubeMesh{};

};


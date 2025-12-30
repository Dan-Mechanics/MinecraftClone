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
#include "../Rendering/OrthographicCamera.h"

class Game {
public:

	Game();
	void setup(GLFWwindow* window, const unsigned int width, const unsigned int height);
	void update(const float deltaTime, const bool hasFocus, GLFWwindow* window, ThreadPool& pool, int& scrollInput);
	void draw(const float deltaTime, const bool hasFocus, GLFWwindow* window);
	void drawDisplay(const float deltaTime, const bool hasFocus, GLFWwindow* window);
	void drawShadows(const float deltaTime, const bool hasFocus, GLFWwindow* window);
	void tick(const float interval, ThreadPool& pool, GLFWwindow* window);
	glm::vec4 getClearColor() const;
	void free();

private:
	unsigned int width{};
	unsigned int height{};
	float worldTickInterval{};
	bool worldToggle{};
	float timer{};

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

	Object crosshair{};
	Object hand{};

	World world{};
	Atlas atlas{};

	Camera camera{};
	OrthographicCamera uiCamera{};

	MouseLook mouseLook{};
	PlayerMovement playerMovement{};

	Terraformer terraformer{};
	BlockSelector blockSelector{};

	Shader materialShader{};
	Shader unlitShader{};
	Shader shadowMapShader{};

	Shader chunkMaterialShader{};
	Shader chunkShadowMapShader{};

	ShadowMapFBO shadowMap{};

	std::vector<Texture> woodMaterial{};
	std::vector<Texture> atlasMaterial{};
	Mesh cubeMesh{};
	Mesh heldCubeMesh{};

};


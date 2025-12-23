#pragma once
#include <glm/ext/vector_float4.hpp>
#include "Object.h"
#include <glm/ext/vector_float3.hpp>
#include "World.h"
#include "ShadowMapFBO.h"
#include "mesh_utils.h"
#include "world_mesh_utils.h"
#include "Terraformer.h"

class Game {
public:

	Game();
	void setup(GLFWwindow* window, const unsigned int width, const unsigned int height);
	void update(const float deltaTime, const bool hasFocus, GLFWwindow* window, ThreadPool& pool);
	void draw(const float deltaTime, const bool hasFocus, GLFWwindow* window);
	void drawDisplay(const float deltaTime, const bool hasFocus, GLFWwindow* window);
	void drawShadows(const float deltaTime, const bool hasFocus, GLFWwindow* window);
	void tick(const float interval, ThreadPool& pool, GLFWwindow* window);
	glm::vec4 getClearColor() const;
	void free();

private:
	glm::vec4 ambientColor{};
	glm::vec4 sunColor{};

	Object sun{};
	Object centerLine{};
	Object center{};
	Object forward{};
	Object right{};
	Object up{};
	Object ground{};

	Object crosshair{};

	World world{};
	Atlas atlas{};
	Camera camera{};
	Camera displayCamera{};
	Terraformer terraformer{};

	Shader materialShader{};
	Shader unlitShader{};
	Shader shadowMapShader{};

	Shader chunkMaterialShader{};
	Shader chunkShadowMapShader{};

	ShadowMapFBO shadowMap{};

	std::vector<Texture> woodMaterial{};
	std::vector<Texture> atlasMaterial{};
	Mesh cubeMesh{};

	float worldTickInterval{};
	float timer{};

};


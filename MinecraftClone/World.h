#pragma once
#include <vector>
#include "world_mesh.h"
#include "Mesh.h"
#include "Object.h"

class World {
public:
	World();
	World(const WORLD_DATA& world, const float maxViewingRange);
	void draw(const std::vector<Texture>& material, const Shader& shader, const Camera& camera, const glm::vec4& lightColor, const glm::vec3& lightPos, const glm::vec4& worldColor);
	void drawForShadowMap(const Shader& shader, const Camera& camera);

	/// <summary>
	/// For the time being.
	/// </summary>
	void generateChunkMeshes(const ATLAS& atlas);
	void free() const;

private:
	WORLD_DATA world{};
	std::vector<Mesh> chunkMeshes{};
	std::vector<glm::vec3> chunkPositions{};
	float maxViewingRange{};
	Object chunkObject{};

};
#include "Chunk.h"

Chunk::Chunk() = default;
Chunk::Chunk(const glm::ivec3& chunkPos) : chunkPos{ chunkPos } {
	// ...
}

Chunk::~Chunk() {
	mesh.free();
}

void Chunk::drawForShadowMap(Object& chunkObject, const Shader& shader, const Camera& camera) const {
	chunkObject.drawAsUnlitColor(mesh, shader, camera);
}

void Chunk::draw(Object& chunkObject, const std::vector<Texture>& material,
	const Shader& shader, const Camera& camera, const glm::vec4& lightColor,
	const glm::vec3& lightPos, const glm::vec4& worldColor) const {
	chunkObject.drawWithMaterial(mesh, material, shader, camera, lightColor, lightPos, worldColor);
}

void Chunk::generateMesh(const ATLAS& atlas, const std::unordered_map<BlockPos, Chunk*>& chunks) {
	mesh.free();
	
	std::vector<Vertex> chunkVerts{};
	std::vector<GLuint> chunkTris{};
	glm::mat4 chunkMatrix{};

	generateChunkMesh(chunkVerts, chunkTris, chunkMatrix, atlas, this, chunks);
	mesh = { chunkVerts, chunkTris, chunkMatrix };
}

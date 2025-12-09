#include "Chunk.h"

Chunk::Chunk() = default;
Chunk::Chunk(const glm::ivec3& chunkPos, const unsigned int chunkSize) : chunkPos{ chunkPos } {
	fillBlocks(chunkSize);
}

Chunk::~Chunk() {
	mesh.free();
}

void Chunk::drawShadows(Object& chunkObject, const Shader& shader, const Camera& camera) const {
	chunkObject.drawAsUnlitColor(mesh, shader, camera);
}

void Chunk::draw(Object& chunkObject, const std::vector<Texture>& material,
	const Shader& shader, const Camera& camera, const glm::vec4& lightColor,
	const glm::vec3& lightPos, const glm::vec4& worldColor) const {
	chunkObject.drawWithMaterial(mesh, material, shader, camera, lightColor, lightPos, worldColor);
}

void Chunk::generateMesh(const Atlas& atlas, const World& world) {
	if (hasMeshed)
		mesh.free();
	
	std::vector<Vertex> chunkVerts{};
	std::vector<GLuint> chunkTris{};
	glm::mat4 chunkMatrix{};

	generateChunkMesh(chunkVerts, chunkTris, chunkMatrix, atlas, this, world);
	mesh = { chunkVerts, chunkTris, chunkMatrix };
	hasMeshed = true;
}

void Chunk::fillBlocks(const unsigned int chunkSize) {
	if (chunkPos.y != -1)
		return;

	for (int x = 0; x < chunkSize; x++) {
		for (int y = 0; y < chunkSize; y++) {
			for (int z = 0; z < chunkSize; z++) {
				if (randomInclusive(0, 1))
					continue;

				glm::ivec3 blockPos{ x, y, z };
				blockPos += chunkPos * (int)chunkSize;
				blocks[blockPos] = static_cast<BlockType>(randomInclusive(0, BlockType::REACTOR));
			}
		}
	}
}

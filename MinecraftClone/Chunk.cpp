#include "Chunk.h"

Chunk::Chunk() = default;
Chunk::Chunk(const glm::ivec3& chunkPos, const int chunkSize) : chunkPos{ chunkPos } {
	generateChunkData(chunkSize);
	//std::cout << "created: " << std::flush;
	//log(chunkPos);
}

Chunk::~Chunk() {
	if (hasMesh)
		mesh.free();

	//std::cout << "destroyed: " << std::flush;
	//log(chunkPos);
}

void Chunk::drawShadows(Object& chunkObject, const Shader& shader, const Camera& camera) const {
	chunkObject.drawAsUnlitColor(mesh, shader, camera);
}

void Chunk::draw(Object& chunkObject, const std::vector<Texture>& material,
	const Shader& shader, const Camera& camera, const glm::vec4& lightColor,
	const glm::vec3& lightPos, const glm::vec4& worldColor) const {
	chunkObject.drawWithMaterial(mesh, material, shader, camera, lightColor, lightPos, worldColor);
}

void Chunk::generateChunkData(const int chunkSize) {
	// FOR THE TIME BEING.
	if (chunkPos.y != -1)
		return;
	
	for (int x = 0; x < chunkSize; x++) {
		for (int y = 0; y < chunkSize; y++) {
			/*for (int z = 0; z < chunkSize; z++) {
				if (randomInclusive(0, 1))
					blocks[glm::ivec3{ x, y, z } + chunkPos * chunkSize] = static_cast<BlockType>(randomInclusive(0, BlockType::REACTOR));
			}*/

			if (randomInclusive(0, 1))
				blocks[glm::ivec3{ x, 0, y } + chunkPos * chunkSize] = static_cast<BlockType>(randomInclusive(0, BlockType::REACTOR));
		}
	}
}

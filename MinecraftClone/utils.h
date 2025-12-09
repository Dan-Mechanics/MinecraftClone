#pragma once
#include <stdlib.h>
#include <vector>
#include "vbo.h"
#include "Texture.h"
#include "Chunk.h"

const glm::ivec3 up{ 0, 1, 0 };
const glm::ivec3 down{ 0, -1, 0 };
const glm::ivec3 left{ -1, 0, 0 };
const glm::ivec3 right{ 1, 0, 0 };
const glm::ivec3 forward{ 0, 0, 1 };
const glm::ivec3 back{ 0, 0, -1 };

int randomInclusive(const int min, const int max);
void log(const glm::vec3& vec);
void log(const glm::ivec3& vec);
void freeMaterial(const std::vector<Texture>& material);

/// <summary>
/// This should work because of rounding lore.
/// </summary>
glm::ivec3 blockPosToChunkPos(const glm::ivec3& blockPos, const unsigned int chunkSize);
bool isChunkValid(const glm::ivec3& chunkPos, const std::unordered_map<glm::ivec3, Chunk*>& chunks);

/// <summary>
/// Does any chunk contain this block position?
/// </summary>
bool has(const glm::ivec3& blockPos, const std::unordered_map<glm::ivec3, Chunk*>& chunks, const unsigned int chunkSize);

namespace std {
	template<> struct hash<glm::ivec3> {
        /// <summary>
        /// https://github.com/Isti01/glCraft/blob/main/src/Util/Util.h
        /// </summary>
        size_t operator()(const glm::ivec3& coord) const noexcept {
            size_t hash = coord.x;
            hash ^= coord.y + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            hash ^= coord.z + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            return hash;
        };
	};
}
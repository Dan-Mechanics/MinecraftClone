#pragma once
#include "minecraft_clone.h"
#include "VBO.h"
#include "BlockType.h"
#include "Direction.h"
#include "hashing_utils.h"
#include "utils.h"
#include "Stamp.h"

/// <summary>
/// https://github.com/Auburn/FastNoiseLite/tree/master/Cpp
/// </summary>
std::vector<int> getHeightMap(const FastNoiseLite& noise, const float height, const int chunkX, const int chunkZ, const int chunkSize);

std::unordered_map<glm::ivec3, BlockType, ivec3hash> fillChunk(const glm::ivec3 chunkPos,
	const int chunkSize, const int waterHeight, const std::vector<int>& heightmap);

std::optional<glm::ivec3> checkKeepChunkLoaded(const glm::ivec3 chunkPos, const glm::ivec3& playerChunkPos, const int rendDist);
Stamp makeTreeStamp(const int height, const int probability, BlockType log, BlockType leaves);
Stamp makeCoreStamp(BlockType blockType);

glm::ivec3 getStandardStampOrigin(const glm::ivec3& chunkPos, const int chunkSize,
	const std::unordered_map<glm::ivec3, std::vector<int>, ivec3hash>& heightMaps);

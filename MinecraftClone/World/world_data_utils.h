#pragma once
#include <vector>
#include "../Rendering/vbo.h"
#include "BlockType.h"
#include "Direction.h"
#include "hashing_utils.h"
#include <unordered_map>
#include "../Core/utils.h"
#include "FastNoiseLite.h"
#include <optional>

/// <summary>
/// https://github.com/Auburn/FastNoiseLite/tree/master/Cpp
/// </summary>
std::vector<int> getHeightMap(const FastNoiseLite& noise, const float height, const int chunkX, const int chunkZ, const int chunkSize);

std::unordered_map<glm::ivec3, BlockType, ivec3hash> fillChunkAsync(const glm::ivec3 chunkPos,
	const int chunkSize, const std::vector<int>& heightMap);

std::optional<glm::ivec3> checkKeepChunkLoaded(const glm::ivec3 chunkPos, const glm::ivec3& playerChunkPos, const int renderDistance);
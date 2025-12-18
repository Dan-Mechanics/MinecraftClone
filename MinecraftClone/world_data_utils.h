#pragma once
#include <vector>
#include "vbo.h"
#include "BlockType.h"
#include "Direction.h"
#include "Atlas.h"
#include "Chunk.h"
#include "hashing_utils.h"
#include <unordered_set>

bool doesChunkGenerateBlocks(const glm::ivec3& chunkPos);
void fillChunkData(std::unordered_map<glm::ivec3, Chunk*, ivec3hash>& chunks, 
	const int chunkSize, const glm::ivec3& chunkPos);

void getWorldChanges(std::vector<glm::ivec3>& chunksToNotify, const glm::ivec3& playerChunkPos,
	const std::unordered_set<glm::ivec3, ivec3hash>& visibleArea, std::unordered_map<glm::ivec3,Chunk*,ivec3hash>& chunks, const int chunkSize);
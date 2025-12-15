#pragma once
#include <vector>
#include "vbo.h"
#include "BlockType.h"
#include "Direction.h"
#include "Atlas.h"
#include "Chunk.h"
#include "hashing_utils.h"
#include <unordered_set>

void allocateChunkData(std::unordered_map<glm::ivec3, Chunk*, ivec3hash>& chunks, const int chunkSize, const glm::ivec3& chunkPos);
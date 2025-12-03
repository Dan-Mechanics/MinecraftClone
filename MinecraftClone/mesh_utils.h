#pragma once
#include "BlockPos.h"
#include <unordered_set>
#include "vbo.h"
#include <unordered_map>
#include "BlockType.h"
#include "Direction.h"

constexpr auto CHUNK_SIZE_FLOAT = 16.0f;
constexpr auto CHUNK_SIZE_INT = 16;

extern const BlockPos UP;
extern const BlockPos DOWN;
extern const BlockPos LEFT;
extern const BlockPos RIGHT;
extern const BlockPos FORWARD;
extern const BlockPos BACK;

void getChunk(std::vector<Vertex>& verts, std::vector<GLuint>& tris, glm::mat4& modelMatrix,
	const std::unordered_map<BlockType, std::vector<std::vector<glm::vec2>>>& atlas,
	const std::unordered_map<BlockPos, BlockType>& chunk,
	const std::unordered_map<BlockPos, std::unordered_map<BlockPos, BlockType>>& allChunks);

bool has(const BlockPos& blockPos, const std::unordered_map<BlockPos, std::unordered_map<BlockPos, BlockType>>& allChunks);

// REMOVE LATER   ----->

void getNyceliumChunk(const std::unordered_set<BlockPos>& blocks, std::vector<Vertex>& verts,
	std::vector<GLuint>& tris, glm::mat4& modelMatrix);

void writeAllAroundFaceUVs(std::vector<Vertex>& verts);
std::vector<glm::vec2> tilePositionToUvs(const int x, const int y);

// END REMOVE LATER < ------

/// <summary>
/// Rule: this can only be called after 4 verts have been added.
/// </summary>
void setCurrentFaceUvs(std::vector<Vertex>& verts, const std::vector<glm::vec2>& uvs);

/// <summary>
/// I hope this works.
/// It should work because rounding lore.
/// </summary>
BlockPos blockPosToChunkPos(const BlockPos& blockPos);

void getPyramid(std::vector<Vertex>& verts, std::vector<GLuint>& tris, glm::mat4& modelMatrix);
void setAllUvs(const std::vector<glm::vec2>& uvs, std::vector<std::vector<glm::vec2>>& allUvs);
void setEquatorUvs(const std::vector<glm::vec2>& uvs, std::vector<std::vector<glm::vec2>>& allUvs);
std::vector<std::vector<glm::vec2>> generateFillUvs();
std::unordered_map<BlockType, std::vector<std::vector<glm::vec2>>> generateAtlas();
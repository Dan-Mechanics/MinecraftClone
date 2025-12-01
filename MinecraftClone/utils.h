#pragma once
#include <vector>
#include "vbo.h"
#include <unordered_set>
#include "BlockPos.h"

extern const BlockPos UP;
extern const BlockPos DOWN;
extern const BlockPos LEFT;
extern const BlockPos RIGHT;
extern const BlockPos FORWARD;
extern const BlockPos BACK;

void getCube(std::vector<Vertex>& verts, std::vector<GLuint>& tris, glm::mat4& modelMatrix);
void getChunk(const std::unordered_set<BlockPos>& blocks, std::vector<Vertex>& verts,
	std::vector<GLuint>& tris, glm::mat4& modelMatrix);

/// <summary>
/// https://stackoverflow.com/questions/40776703/converting-from-radians-to-degrees
/// </summary>
int randomInclusive(const int min, const int max);

/// <summary>
/// Given that they are always the same.
/// </summary>
void writeAllAroundFaceUVs(std::vector<Vertex>& verts);

void getPyramid(std::vector<Vertex>& verts, std::vector<GLuint>& tris, glm::mat4& modelMatrix);
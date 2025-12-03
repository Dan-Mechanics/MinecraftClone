#pragma once
#include <vector>
#include "vbo.h"
#include <unordered_set>
#include "BlockPos.h"
#include "BlockType.h"
#include "Direction.h"
#include <unordered_map>

extern const BlockPos UP;
extern const BlockPos DOWN;
extern const BlockPos LEFT;
extern const BlockPos RIGHT;
extern const BlockPos FORWARD;
extern const BlockPos BACK;

extern const std::unordered_map<BlockType, std::vector<glm::vec2>> map;

void getCube(std::vector<Vertex>& verts, std::vector<GLuint>& tris, glm::mat4& modelMatrix);
void getChunk(const std::unordered_set<BlockPos>& blocks, std::vector<Vertex>& verts,
	std::vector<GLuint>& tris, glm::mat4& modelMatrix);

std::vector<glm::vec2> tilePositionToUvs(const int x, const int y);

/// <summary>
/// https://stackoverflow.com/questions/40776703/converting-from-radians-to-degrees
/// </summary>
int randomInclusive(const int min, const int max);

/// <summary>
/// Given that they are always the same.
/// </summary>
void writeAllAroundFaceUVs(std::vector<Vertex>& verts);

/// <summary>
/// Rule: this can only be called after 4 verts have been added.
/// </summary>
void setCurrentFaceUvs(std::vector<Vertex>& verts, const std::vector<glm::vec2>& uvs);

void getPyramid(std::vector<Vertex>& verts, std::vector<GLuint>& tris, glm::mat4& modelMatrix);
void setAllUvs(const std::vector<glm::vec2>& uvs, std::vector<std::vector<glm::vec2>>& allUvs);
void setEquatorUvs(const std::vector<glm::vec2>& uvs, std::vector<std::vector<glm::vec2>>& allUvs);
std::vector<std::vector<glm::vec2>> generateFillUvs();
std::unordered_map<BlockType, std::vector<std::vector<glm::vec2>>> generateAtlas();
#pragma once
#include <vector>
#include "vbo.h"
#include <unordered_set>

void getCube(std::vector<Vertex>& verts, std::vector<GLuint>& tris);
void getChunk(const std::unordered_set<glm::ivec3>& blocks, std::vector<Vertex>& verts, std::vector<GLuint>& tris);

/// <summary>
/// Given that they are always the same.
/// </summary>
void writeAllAroundFaceUVs(std::vector<Vertex>& verts);

void getPyramid(std::vector<Vertex>& verts, std::vector<GLuint>& tris);
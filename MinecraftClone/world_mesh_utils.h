#pragma once
#include <vector>
#include "vbo.h"
#include "BlockType.h"
#include "Direction.h"
#include "Atlas.h"

void generateChunkMesh(std::vector<Vertex>& verts, std::vector<GLuint>& tris, glm::mat4& modelMatrix,
	const Atlas& atlas, const Chunk& chunk, const std::unordered_map<glm::ivec3, Chunk>& chunks);

/// <summary>
/// This UV code is a little strange but it works.
/// </summary>
Face tilePositionToUVs(const int x, const int y);

void scaleUVs(Face& face, const float scale);
void translateUVs(Face& face, const float x, const float y);

/// <summary>
/// https://stackoverflow.com/questions/26346056/move-first-element-of-vector-to-last-element
/// </summary>
void rotateUVsClockwise(Face& face);

/// <summary>
/// Rule: this can only be called after 4 verts have been added.
/// </summary>
void setCurrentFaceUVs(std::vector<Vertex>& verts, const Face& face);
void setEquatorUVs(Map& map, const Face& face);
void setPoleUVs(Map& map, const Face& face);

Map generateUniformMap(const Face& face);
Map generateEmptyMap();

Atlas generateAtlas();

glm::ivec3 posToBlockPos(const glm::vec3& pos);

/// <summary>
/// Rounds down.
/// </summary>
glm::ivec3 blockPosToChunkPos(const glm::ivec3& blockPos, const unsigned int chunkSize);
bool isChunkValid(const glm::ivec3& chunkPos, const std::unordered_map<glm::ivec3, Chunk>& chunks);

/// <summary>
/// Does any chunk contain this block position?
/// </summary>
bool has(const glm::ivec3& blockPos, const std::unordered_map<glm::ivec3, Chunk>& chunks, const unsigned int chunkSize);
#pragma once
#include <vector>
#include "../Rendering/vbo.h"
#include "BlockType.h"
#include "Direction.h"
#include "Atlas.h"
#include "Chunk.h"
#include "hashing_utils.h"
#include <unordered_set>

void generateChunkMesh(std::vector<ChunkVertex>& verts, std::vector<GLuint>& tris,
	const int chunkSize, const Atlas& atlas, const glm::ivec3& chunkPos,
	const std::unordered_map<glm::ivec3, Chunk*, ivec3hash>& chunks);

void setCubeFacesAsBlockType(std::vector<Vertex>& verts, const Atlas& atlas, const BlockType blockType);

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
void setCurrentFaceUVs(std::vector<ChunkVertex>& verts, const Face& face);
void setEquatorUVs(Map& map, const Face& face);
void setPoleUVs(Map& map, const Face& face);

Map generateUniformMap(const Face& face);
Map generateEmptyMap();

Atlas generateAtlas();

glm::ivec3 posToBlockPos(const glm::vec3& pos);

/// <summary>
/// Rounds down.
/// </summary>
glm::ivec3 blockPosToChunkPos(const glm::ivec3& blockPos, const int chunkSize);

/// <summary>
/// Does any chunk contain this block position?
/// </summary>
bool has(const glm::ivec3& blockPos, const std::unordered_map<glm::ivec3, Chunk*, ivec3hash>& chunks, const int chunkSize);

/// <summary>
/// Assume there is a chunk
/// when the data hasn't loaded in yet.
/// </summary>
bool assumptiveHas(const glm::ivec3& blockPos, const std::unordered_map<glm::ivec3, Chunk*, ivec3hash>& chunks, const int chunkSize);
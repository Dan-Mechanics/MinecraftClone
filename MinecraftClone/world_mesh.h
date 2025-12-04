#pragma once
#include "BlockPos.h"
#include <unordered_set>
#include "vbo.h"
#include <unordered_map>
#include "BlockType.h"
#include "Direction.h"

#define CHUNK std::unordered_map<BlockPos, BlockType>
#define WORLD std::unordered_map<BlockPos, CHUNK>

#define MAP std::vector<std::vector<glm::vec2>>
#define ATLAS std::unordered_map<BlockType, MAP>

void generateChunkMesh(std::vector<Vertex>& verts, std::vector<GLuint>& tris, glm::mat4& modelMatrix,
	const ATLAS& atlas, const CHUNK& chunk, const WORLD& world);

bool has(const BlockPos& blockPos, const WORLD& world);

/// <summary>
/// https://github.com/samhogan/Minecraft-Unity3D/blob/master/Assets/Scripts/TilePos.cs
/// </summary>
std::vector<glm::vec2> tilePositionToUVs(const unsigned int x, const unsigned int y);

void scaleUVs(std::vector<glm::vec2>& uvs, const float scale);
void translateUVs(std::vector<glm::vec2>& uvs, const float x, const float y);
std::vector<glm::vec2> getDebugUVs();


/// <summary>
/// Rule: this can only be called after 4 verts have been added.
/// </summary>
void setCurrentFaceUVs(std::vector<Vertex>& verts, const std::vector<glm::vec2>& uvs);

/// <summary>
/// This should work because of rounding lore.
/// </summary>
BlockPos blockPosToChunkPos(const BlockPos& blockPos);

void setEquatorUVs(MAP& map, const std::vector<glm::vec2>& to);
void setPoleUVs(MAP& map, const std::vector<glm::vec2>& to);

MAP generateUniformMap(const std::vector<glm::vec2>& to);
MAP generateFillerMap();
ATLAS generateAtlas();

/// <summary>
/// Placeholder code.
/// </summary>
WORLD generateDemoWorld();
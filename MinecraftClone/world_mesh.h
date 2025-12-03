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

extern const BlockPos UP;
extern const BlockPos DOWN;
extern const BlockPos LEFT;
extern const BlockPos RIGHT;
extern const BlockPos FORWARD;
extern const BlockPos BACK;

void getChunkMesh(std::vector<Vertex>& verts, std::vector<GLuint>& tris, glm::mat4& modelMatrix,
	const ATLAS& atlas, const CHUNK& chunk, const WORLD& world);

bool has(const BlockPos& blockPos, const WORLD& world);

/// <summary>
/// https://github.com/samhogan/Minecraft-Unity3D/blob/master/Assets/Scripts/TilePos.cs
/// </summary>
std::vector<glm::vec2> tilePositionToUvs(const int x, const int y);

/// <summary>
/// Rule: this can only be called after 4 verts have been added.
/// </summary>
void setCurrentFaceUvs(std::vector<Vertex>& verts, const std::vector<glm::vec2>& uvs);

/// <summary>
/// This should work because of rounding lore.
/// </summary>
BlockPos blockPosToChunkPos(const BlockPos& blockPos);

void setAllUvs(MAP& map, const std::vector<glm::vec2>& to);
void setEquatorUvs(MAP& map, const std::vector<glm::vec2>& to);
void setPoleUvs(MAP& map, const std::vector<glm::vec2>& to);

std::vector<std::vector<glm::vec2>> generateFillerMap();
ATLAS generateAtlas();

/// <summary>
/// Placeholder code.
/// </summary>
WORLD generateWorld();
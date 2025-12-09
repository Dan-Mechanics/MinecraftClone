#pragma once
#include <vector>
#include "vbo.h"
#include "Chunk.h"
#include "BlockType.h"
#include "utils.h"
#include "Direction.h"
#include "World.h"

struct Face {
public:
	std::vector<glm::vec2> uvs{};
	Face() = default;
	Face(const std::vector<glm::vec2>& uvs) : uvs(uvs) { }

};

struct Map {
public:
	std::vector<Face> faces{};
	Map() = default;
	Map(const std::vector<Face>& faces) : faces(faces) { }

};

struct Atlas {
public:
	std::unordered_map<BlockType, Map> maps{};
	Atlas() = default;
	Atlas(const std::unordered_map<BlockType, Map>& maps) : maps(maps) { }

};

void generateChunkMesh(std::vector<Vertex>& verts, std::vector<GLuint>& tris, glm::mat4& modelMatrix,
	const Atlas& atlas, const Chunk& chunk, const World& world);

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

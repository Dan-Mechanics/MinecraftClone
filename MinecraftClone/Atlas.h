#pragma once
#include <unordered_map>

struct Face {
public:
	std::vector<glm::vec2> uvs{};
	Face() = default;
	Face(const std::vector<glm::vec2>& uvs) : uvs(uvs) {}

};

struct Map {
public:
	std::vector<Face> faces{};
	Map() = default;
	Map(const std::vector<Face>& faces) : faces(faces) {}

};

struct Atlas {
public:
	std::unordered_map<BlockType, Map> maps{};
	Atlas() = default;
	Atlas(const std::unordered_map<BlockType, Map>& maps) : maps(maps) {}

};
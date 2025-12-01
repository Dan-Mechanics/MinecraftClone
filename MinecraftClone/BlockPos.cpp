#include "BlockPos.h"

const BlockPos UP = { 0, 1, 0 };
const BlockPos DOWN = { 0, -1, 0 };
const BlockPos LEFT = { -1, 0, 0 };
const BlockPos RIGHT = { 1, 0, 0 };
const BlockPos FORWARD = { 0, 0, 1 };
const BlockPos BACK = { 0, 0, -1 };

BlockPos::BlockPos() = default;
BlockPos::BlockPos(const int& x, const int& y, const int& z) : x{ x }, y{ y }, z{ z } { }
BlockPos::BlockPos(const BlockPos& other) : x{ other.x }, y{ other.y }, z{ other.z } { }

BlockPos& BlockPos::operator=(const BlockPos& other) {
	x = other.x;
	y = other.y;
	z = other.z;
	return *this;
}

bool BlockPos::operator==(const BlockPos& other) const {
	return x == other.x && y == other.y && z == other.z;
}

bool BlockPos::operator<(const BlockPos& other) {
	if (x < other.x)
		return true;

	return *this == other;
}

size_t BlockPos::operator()(const BlockPos& pointToHash) const noexcept {
	size_t hash = pointToHash.x + 10 * pointToHash.y + 20 * pointToHash.z;
	return hash;
}

BlockPos BlockPos::operator+(const BlockPos& other) const {
	return { x + other.x, y + other.y, z + other.z };
}

glm::ivec3 BlockPos::getVec3() const {
	return { x, y, z };
}


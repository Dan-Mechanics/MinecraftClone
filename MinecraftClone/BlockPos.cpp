#include "BlockPos.h"
#include <string>

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

BlockPos BlockPos::operator+(const BlockPos& other) const {
	return { x + other.x, y + other.y, z + other.z };
}

glm::ivec3 BlockPos::getVec3() const {
	return { x, y, z };
}


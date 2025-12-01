#pragma once
#include <glm/glm.hpp>

extern const BlockPos UP;
extern const BlockPos DOWN;
extern const BlockPos LEFT;
extern const BlockPos RIGHT;
extern const BlockPos FORWARD;
extern const BlockPos BACK;

/// <summary>
/// https://stackoverflow.com/questions/50888127/how-can-i-use-an-unordered-set-with-a-custom-struct
/// </summary>
struct BlockPos {
public:
	int x{};
	int y{};
	int z{};
	BlockPos();
	BlockPos(const int& x, const int& y, const int& z);
	BlockPos(const BlockPos& other);
	BlockPos& operator=(const BlockPos& other);
	bool operator==(const BlockPos& other) const;
	bool operator<(const BlockPos& other); 
	size_t operator()(const BlockPos& pointToHash) const noexcept;

	BlockPos operator+(const BlockPos& other) const;
	glm::ivec3 getVec3() const;
};


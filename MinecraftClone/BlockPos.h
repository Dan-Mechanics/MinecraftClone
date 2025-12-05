#pragma once
#include <glm/glm.hpp>

/// <summary>
/// https://stackoverflow.com/questions/50888127/how-can-i-use-an-unordered-set-with-a-custom-struct
/// https://en.cppreference.com/w/cpp/utility/hash.html
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

	BlockPos operator+(const BlockPos& other) const;
	glm::vec3 getVec3() const;
};

namespace std {
	template<> struct hash<BlockPos> {
		//std::size_t operator()(const BlockPos& blockPos) const noexcept {
		//	std::size_t h1 = std::hash<int>{}(blockPos.x);
		//	std::size_t h2 = std::hash<int>{}(blockPos.y);
		//	std::size_t h3 = std::hash<int>{}(blockPos.z);

		//	std::size_t c1 = h1 ^ (h2 << 1);
		//	return c1 ^ (h3 << 1); // OR USE BOOST::HASH_COMBINE.
		//}

		std::size_t operator()(BlockPos blockPos) const {
			std::size_t seed = 3;

			blockPos.x = ((blockPos.x >> 16) ^ blockPos.x) * 0x45d9f3b;
			blockPos.x = ((blockPos.x >> 16) ^ blockPos.x) * 0x45d9f3b;
			blockPos.x = (blockPos.x >> 16) ^ blockPos.x;
			seed ^= blockPos.x + 0x9e3779b9 + (seed << 6) + (seed >> 2);

			blockPos.y = ((blockPos.y >> 16) ^ blockPos.y) * 0x45d9f3b;
			blockPos.y = ((blockPos.y >> 16) ^ blockPos.y) * 0x45d9f3b;
			blockPos.y = (blockPos.y >> 16) ^ blockPos.y;
			seed ^= blockPos.y + 0x9e3779b9 + (seed << 6) + (seed >> 2);

			blockPos.z = ((blockPos.z >> 16) ^ blockPos.z) * 0x45d9f3b;
			blockPos.z = ((blockPos.z >> 16) ^ blockPos.z) * 0x45d9f3b;
			blockPos.z = (blockPos.z >> 16) ^ blockPos.z;
			seed ^= blockPos.z + 0x9e3779b9 + (seed << 6) + (seed >> 2);

			return seed;
		}
	};
}
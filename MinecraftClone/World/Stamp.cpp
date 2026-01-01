#include "Stamp.h"

Stamp::Stamp() = default;
Stamp::Stamp(std::unordered_map<glm::ivec3, BlockType, ivec3hash>& other)
	: blocks{ std::move(other) } { }
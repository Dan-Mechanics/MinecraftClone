#pragma once
#include <glm/ext/vector_int3.hpp>

class hasing_utils {
    hasing_utils() = default;

public:
    class HashVec3 {
    public:
        size_t operator()(const glm::ivec3& coord) const noexcept {
            size_t hash = coord.x;
            hash ^= coord.y + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            hash ^= coord.z + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            return hash;
        };
    };
};
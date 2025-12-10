#pragma once

/// <summary>
/// https://github.com/Isti01/glCraft/blob/main/src/Util/Util.h
/// </summary>
class IntVec3Hash {
public:
    size_t operator()(const glm::ivec3& coord) const noexcept {
        size_t hash = coord.x;
        hash ^= coord.y + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        hash ^= coord.z + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        return hash;
    };

};
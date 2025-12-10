#pragma once

const glm::ivec3 up{ 0, 1, 0 };
const glm::ivec3 down{ 0, -1, 0 };
const glm::ivec3 left{ -1, 0, 0 };
const glm::ivec3 right{ 1, 0, 0 };
const glm::ivec3 forward{ 0, 0, 1 };
const glm::ivec3 back{ 0, 0, -1 };

enum Direction {
    UP = 0,
    DOWN = 1,
    FORWARD = 2,
    RIGHT = 3,
    BACK = 4,
    LEFT = 5
};
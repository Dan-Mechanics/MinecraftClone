#pragma once

const glm::ivec3 up{ 0, 1, 0 };
const glm::ivec3 down = -up;

const glm::ivec3 right{ 1, 0, 0 };
const glm::ivec3 left = -right;

const glm::ivec3 forward{ 0, 0, 1 };
const glm::ivec3 back = -forward;

// ===

const glm::vec3 worldOrigin{ 0.0f, 0.0f, 0.0f };

const glm::vec3 worldUp{ 0.0f, 1.0f, 0.0f };
const glm::vec3 worldDown = -worldUp;

const glm::vec3 worldRight{ 1.0f, 0.0f, 0.0f };
const glm::vec3 worldLeft = -worldRight;

const glm::vec3 worldForward{ 0.0f, 0.0f, 1.0f };
const glm::vec3 worldBack = -worldForward;

// ===

enum Direction {
    UP = 0,
    DOWN = 1,
    FORWARD = 2,
    RIGHT = 3,
    BACK = 4,
    LEFT = 5
};

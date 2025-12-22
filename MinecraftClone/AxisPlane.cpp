#include "AxisPlane.h"

AxisPlane::AxisPlane(glm::vec3 normal, glm::vec3 origin, glm::vec3 direction)
    : normal(normal), origin(origin), direction(direction) {
    planeExtent = std::floor(glm::dot(normal, origin)) + (glm::dot(normal, direction) > 0 ? 1.0f : 0.0f);
    planeDirection = glm::dot(normal, direction) < 0 ? -1.0f : 1.0f;

    getPoint(point);
    distance = getDistance();
}

bool AxisPlane::getPoint(glm::vec3& point) const {
    float extent;
    if (!intersect(extent))
        return false;
    
    point = origin + (extent * direction);
    return true;
}

bool AxisPlane::intersect(float& extent) const {
    extent = 0.0f;
    const auto dot = glm::dot(normal, direction);
    if (dot == 0.0f)
        return false;

    extent = glm::dot(normal, normal * planeExtent - origin) / dot;
    return true;
}

float AxisPlane::getDistance() const {
    return glm::distance(origin, point);
}

void AxisPlane::extendForward() {
    planeExtent += planeDirection;
    getPoint(point);
    distance = getDistance();
}

bool AxisPlane::getBlockPos(const glm::vec3& a, const glm::vec3& b, glm::ivec3& blockPos) {
    glm::vec3 diff = glm::abs(a - b);
    if (diff.x > 1.001f || diff.y > 1.001f || diff.z > 1.001f)
        return false;

    blockPos = glm::floor((a + b) / 2.0f);
    return true;
}
#include "AxisPlane.h"

AxisPlane::AxisPlane(glm::vec3 planeNormal, glm::vec3 origin, glm::vec3 direction)
    : planeNormal{ planeNormal }, origin{ origin }, direction{ direction } {
    planeOffsetMovement = glm::dot(planeNormal, direction) < 0 ? -1.0f : 1.0f;
    planeOffset = std::floor(glm::dot(planeNormal, origin)) + (glm::dot(planeNormal, direction) > 0 ? 1.0f : 0.0f);

    point = calculatePoint();
    distance = calculateDistance();
}

glm::vec3 AxisPlane::calculatePoint() const {
    float dist = intersect();
    if (dist < 0.0f)
        return glm::vec3(std::numeric_limits<float>::infinity());

    return origin + (dist * direction);
}

float AxisPlane::intersect() const {
    const auto dot = glm::dot(planeNormal, direction);
    if (dot == 0.0f)
        return -std::numeric_limits<float>::infinity();

    return glm::dot(planeNormal, planeNormal * planeOffset - origin) / dot;
}

float AxisPlane::calculateDistance() const { 
    return glm::distance(origin, point);
}

/// <summary>
/// This is so that we know the order
/// to snake down the grid in.
/// </summary>
bool AxisPlane::operator<(const AxisPlane& other) const { 
    return distance < other.distance;
}

void AxisPlane::advance() {
    planeOffset += planeOffsetMovement;
    point = calculatePoint();
    distance = calculateDistance();
}
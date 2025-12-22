#pragma once
#include <glm/ext/vector_float3.hpp>
#include <optional>
#include <glm/ext/vector_int3.hpp>
#include <glm/geometric.hpp>

/// <summary>
/// CREDIT: https://github.com/Isti01/glCraft/blob/main/src/Math/WorldRayCast.cpp
/// </summary>
struct AxisPlane {
public:
	AxisPlane(glm::vec3 normal, glm::vec3 origin, glm::vec3 direction);

	bool getBlockPos(const glm::vec3 & a, const glm::vec3 & b, glm::ivec3& blockPos);
	glm::vec3 getPoint() const { return point; };
	float getDistance() const { return distance; };
	bool operator<(const AxisPlane& other) const { return distance < other.distance; }
	void extendForward();

private:
	glm::vec3 normal;

	float planeDirection;
	float planeExtent;

	glm::vec3 origin;
	glm::vec3 direction;

	glm::vec3 point;
	float distance;

	bool intersect(float& result) const;
	float getDistance() const;
	bool getPoint(glm::vec3& point) const;
};
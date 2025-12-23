#pragma once
#include <glm/ext/vector_float3.hpp>
#include <optional>
#include <glm/ext/vector_int3.hpp>
#include <glm/geometric.hpp>

/// <summary>
/// CREDIT: https://github.com/Isti01/glCraft/blob/main/src/Math/AxisPlane.h
/// </summary>
struct AxisPlane {
public:
	glm::vec3 point{};
	float distance{};

	AxisPlane(glm::vec3 planeNormal, glm::vec3 origin, glm::vec3 direction);
	bool operator<(const AxisPlane& other) const;
	void advance();

private:
	glm::vec3 planeNormal{};
	float planeOffsetMovement{};
	float planeOffset{};

	glm::vec3 origin{};
	glm::vec3 direction{};

	/// <summary>
	/// Check for next intersection.
	/// </summary>
	/// <returns>Distance to next.</returns>
	float intersect() const;

	/// <summary>
	/// Total ray length.
	/// </summary>
	float calculateDistance() const;

	/// <summary>
	/// Ray intersection point.
	/// </summary>
	glm::vec3 calculatePoint() const;

};
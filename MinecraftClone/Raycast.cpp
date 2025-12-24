#include "Raycast.h"

Raycast::Raycast() = default;
Raycast::Raycast(const glm::vec3& origin, glm::vec3 direction, float range)
	: origin{ origin }, direction{ direction }, range{ range } { }
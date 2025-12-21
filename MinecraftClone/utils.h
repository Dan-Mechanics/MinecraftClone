#pragma once
#include <stdlib.h>
#include <iostream>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_int3.hpp>
#include <vector>
#include "Texture.h"
#include <string>
#include "Direction.h"

// TODO: ADD NAMESPACE HERE.

int randomInclusive(const int min, const int max);
void logVec3(const glm::vec3& vec);
void logIvec3(const glm::ivec3& vec);
std::string ivec3ToString(const glm::ivec3& vec);
void freeMaterial(const std::vector<Texture>& material);
void bindMaterial(std::vector<Texture>& material, const Shader& shader);
Direction getDirectionFromVector(const glm::vec3& vec);
glm::ivec3 directionToIvec3(const Direction dir);
#pragma once
#include <stdlib.h>
#include <iostream>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_int3.hpp>
#include <vector>
#include "Texture.h"
#include <string>

int randomInclusive(const int min, const int max);
void log(const glm::vec3& vec);
void log(const glm::ivec3& vec);
std::string toString(const glm::ivec3& vec);
void freeMaterial(const std::vector<Texture>& material);
void bindMaterial(std::vector<Texture>& material, const Shader& shader);
#pragma once
#include <stdlib.h>
//#include <iostream>
#include <vector>
#include "vbo.h"
#include "Texture.h"

/// <summary>
/// https://stackoverflow.com/questions/40776703/converting-from-radians-to-degrees
/// </summary>
int randomInclusive(const int min, const int max);

void logVec3(const glm::vec3& vec);

void getCubeMesh(std::vector<Vertex>& verts, std::vector<GLuint>& tris, glm::mat4& modelMatrix);
void getPyramidMesh(std::vector<Vertex>& verts, std::vector<GLuint>& tris, glm::mat4& modelMatrix);
void freeMaterial(const std::vector<Texture>& material);
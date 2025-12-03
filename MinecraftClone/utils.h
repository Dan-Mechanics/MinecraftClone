#pragma once
#include <stdlib.h>
//#include <iostream>
#include <vector>
#include "vbo.h"

/// <summary>
/// https://stackoverflow.com/questions/40776703/converting-from-radians-to-degrees
/// </summary>
int randomInclusive(const int min, const int max);

void getCube(std::vector<Vertex>& verts, std::vector<GLuint>& tris, glm::mat4& modelMatrix);
void getPyramid(std::vector<Vertex>& verts, std::vector<GLuint>& tris, glm::mat4& modelMatrix);
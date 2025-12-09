#pragma once
#include <stdlib.h>
#include <vector>
#include "vbo.h"
#include "Texture.h"

void getCubeMesh(std::vector<Vertex>& verts, std::vector<GLuint>& tris, glm::mat4& modelMatrix);
void getPyramidMesh(std::vector<Vertex>& verts, std::vector<GLuint>& tris, glm::mat4& modelMatrix);
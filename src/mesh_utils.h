#pragma once
#include "minecraft_clone.h"
#include "VBO.h"
#include "Texture.h"

void getCubeMesh(std::vector<Vertex>& verts, std::vector<GLuint>& tris, glm::mat4& modelMatrix);
void getPyramidMesh(std::vector<Vertex>& verts, std::vector<GLuint>& tris, glm::mat4& modelMatrix);
#pragma once
#include "Mesh.h"
#include "Object.h"
#include "utils.h"

class Game {
public:
	glm::vec4 skyColor;
	glm::vec4 sunColor;
	GLuint width{};
	GLuint height{};
	double fpsCap = 300.0;
	double tickInterval = 0.02;
};


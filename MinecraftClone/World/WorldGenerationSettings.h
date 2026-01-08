#pragma once
#include "FastNoiseLite.h"
#include "Stamp.h"
#include "world_data_utils.h"

struct WorldGenerationSettings {
public:
	FastNoiseLite noise{};
	float height{};
	int waterHeight{};
	Stamp blueTree{};
	Stamp ashTree{};
	Stamp steelCore{};

	/// <summary>
	/// https://github.com/Isti01/glCraft/blob/main/src/World/WorldGenerator.cpp
	/// </summary>
	WorldGenerationSettings();

};


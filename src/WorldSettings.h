#pragma once
#include "minecraft_clone.h"
#include "Stamp.h"
#include "world_data_utils.h"

struct WorldSettings {
public:
	int chunkSize{};
	int rendDist{};
	int smallRendDist{};
	int largeRendDist{};

	WorldSettings();
	WorldSettings(const int chunkSize, const int rendDist);

};


#include "WorldSettings.h"

WorldSettings::WorldSettings() = default;
WorldSettings::WorldSettings(const int chunkSize, const int rendDist)
	: rendDist{rendDist}, chunkSize{chunkSize} {
	smallRendDist = rendDist - 1;
	largeRendDist = rendDist + 1;
}
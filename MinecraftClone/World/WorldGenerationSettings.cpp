#include "WorldGenerationSettings.h"

WorldGenerationSettings::WorldGenerationSettings() {
	noise.SetFractalOctaves(3);
	noise.SetFractalLacunarity(4.0f);
	noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	noise.SetFractalType(FastNoiseLite::FractalType_FBm);

	height = 25.0f;
	waterHeight = -7;

	blueTree = makeTreeStamp(14, 9, BlockType::LOG, BlockType::LEAVES);
	ashTree = makeTreeStamp(14, 9, BlockType::ASH_LOG, BlockType::GLOW_BERRIES);
}
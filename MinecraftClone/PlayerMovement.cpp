#include "PlayerMovement.h"

PlayerMovement::PlayerMovement() = default;

void PlayerMovement::update(const std::unordered_map<glm::ivec3, Chunk*, ivec3hash>& chunks, const int chunkSize) {
	// ISOLATE THE PLAYER'S MOTION INTO ESSENTIAL ELEMENTS
	// BOX CAST EACH ELEMENT
	// CLAMP THE ELEMENTS THAT FAIL
	
	
	const auto blockPos = posToBlockPos(pos);
	if (!has(blockPos, chunks, chunkSize)) {

	}

	if (blockPos != prevBlockPos)
		prevBlockPos = blockPos;
}
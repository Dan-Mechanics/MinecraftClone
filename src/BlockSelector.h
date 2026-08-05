#pragma once
#include "minecraft_clone.h"
#include "BlockType.h"

class BlockSelector {
public:
	BlockSelector();
	bool onScroll(int& scrollInput);
	BlockType getBlockType() const;

private:
	int index{};

};


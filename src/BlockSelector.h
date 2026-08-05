#pragma once
#include <GLFW/glfw3.h>
#include "../World/BlockType.h"

class BlockSelector {
public:
	BlockSelector();
	bool onScroll(int& scrollInput);
	BlockType getBlockType() const;

private:
	int index{};

};


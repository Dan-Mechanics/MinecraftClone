#pragma once
#include <GLFW/glfw3.h>
#include "BlockType.h"

class BlockSelector {
public:
	BlockSelector();
	void onScroll(int& scrollInput);


private:
	int index{};
	BlockType current{};
};


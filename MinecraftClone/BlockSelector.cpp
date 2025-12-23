#include "BlockSelector.h"

BlockSelector::BlockSelector() {
	index = 0;
	current = static_cast<BlockType>(index);
}

void BlockSelector::onScroll(int& scrollInput) {
	// ORDER !!
	if (scrollInput > 0) {
		scrollInput--;
		index--;
		if (index < 0)
			index = BlockType::END - index;
	}
	else if (scrollInput < 0) {
		scrollInput++;
		index++;
		if (index >= BlockType::END) 
			index = BlockType::END - 1;
	}

	current = static_cast<BlockType>(index);
}

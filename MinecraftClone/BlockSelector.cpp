#include "BlockSelector.h"

BlockSelector::BlockSelector() = default;

bool BlockSelector::onScroll(int& scrollInput) {
	if (scrollInput > 0) {
		scrollInput--;
		index++;
		if (index >= BlockType::END)
			index = 0;

		return true;
	}

	if (scrollInput < 0) {
		scrollInput++;
		index--;
		if (index < 0)
			index = BlockType::END + index;
		
		return true;
	}

	return false;
}

BlockType BlockSelector::getBlockType() const {
	return static_cast<BlockType>(index);
}

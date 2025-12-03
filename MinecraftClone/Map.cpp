#include "Map.h"

Map::Map() = default;

Map::Map(const std::vector<glm::vec2>& upUvs,
	const std::vector<glm::vec2>& downUvs,
	const std::vector<glm::vec2>& forwardUvs,
	const std::vector<glm::vec2>& rightUvs,
	const std::vector<glm::vec2>& backUvs,
	const std::vector<glm::vec2>& leftUvs)
	: upUvs(upUvs), downUvs(downUvs), forwardUvs(forwardUvs),
	rightUvs(rightUvs), backUvs(backUvs), leftUvs(leftUvs) { }

Map::Map(const std::vector<glm::vec2>& upUvs)
	: upUvs(upUvs) { }

Map::Map(const std::vector<glm::vec2>& upUvs, const std::vector<glm::vec2>& downUvs, const std::vector<glm::vec2>& forwardUvs)
	: upUvs(upUvs), downUvs(downUvs), forwardUvs(forwardUvs) { }

void Map::setAroundToForward() {
	rightUvs = forwardUvs;
	backUvs = forwardUvs;
	leftUvs = forwardUvs;
}

void Map::setAllToUp() {
	downUvs = upUvs;
	forwardUvs = upUvs;
	rightUvs = upUvs;
	backUvs = upUvs;
	leftUvs = upUvs;
}

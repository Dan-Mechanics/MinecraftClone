#pragma once
#include <vector>
#include <glm/glm.hpp>

struct Map {
public:
	std::vector<glm::vec2> upUvs{};
	std::vector<glm::vec2> downUvs{};
	std::vector<glm::vec2> forwardUvs{};
	std::vector<glm::vec2> rightUvs{};
	std::vector<glm::vec2> backUvs{};
	std::vector<glm::vec2> leftUvs{};

	Map();
	Map(const std::vector<glm::vec2>& upUvs,
		const std::vector<glm::vec2>& downUvs,
		const std::vector<glm::vec2>& forwardUvs,
		const std::vector<glm::vec2>& rightUvs,
		const std::vector<glm::vec2>& backUvs,
		const std::vector<glm::vec2>& leftUvs);

	Map(const std::vector<glm::vec2>& upUvs);
	Map(const std::vector<glm::vec2>& upUvs,
		const std::vector<glm::vec2>& downUvs,
		const std::vector<glm::vec2>& forwardUvs);

	void setAroundToForward();
	void setAllToUp();

};


#pragma once
struct Timer {
public:
	float interval{};
	float value{};

	Timer();
	Timer(const float interval);

	bool tick(const float deltaTime);

};
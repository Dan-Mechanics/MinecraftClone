#include "Timer.h"

Timer::Timer() = default;
Timer::Timer(const float interval) {
	if (interval > 0.0f) {
		this->interval = interval;
	}
	else {
		this->interval = 0.01f;
	}
}

bool Timer::tick(const float deltaTime) {
	value += deltaTime;
	if (value >= interval) {
		value = 0.0f;
		return true;
	}

	return false;
}

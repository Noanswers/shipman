#pragma once
#include <chrono>
#include <functional>

class CMyChrono
{
public:
	CMyChrono() = default;
	~CMyChrono() = default;

	static void afterTime(float seconds, std::function<void(void)> closure);
};


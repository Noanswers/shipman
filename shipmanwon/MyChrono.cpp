#include "stdafx.h"
#include "MyChrono.h"

void CMyChrono::afterTime(float seconds, std::function<void(void)> closure)
{
	auto startTime = std::chrono::system_clock::now();
	
	std::chrono::milliseconds duration;
	do{
		auto endTime = std::chrono::system_clock::now();
		duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	} while (duration.count() < seconds * 1000);

	closure();
}

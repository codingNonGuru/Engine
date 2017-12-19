#include <chrono>
#include <iostream>

#include "Time.hpp"

float Time::delta_ = 0.0f;

double Time::lastTimeStamp_ = 0.0;

double Time::currentTimeStamp_ = 0.0;

void Time::Update()
{
	using namespace std::chrono;
	milliseconds timeStamp = duration_cast< milliseconds >(system_clock::now().time_since_epoch());

	lastTimeStamp_ = currentTimeStamp_;

	currentTimeStamp_ = timeStamp.count() / 1000.0;

	delta_ = (float)(currentTimeStamp_ - lastTimeStamp_);
}

float Time::GetDelta()
{
	return delta_;
}

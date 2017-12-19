#pragma once

class Time
{
private:
	static float delta_;

	static double lastTimeStamp_;

	static double currentTimeStamp_;

public:
	static void Update();

	static float GetDelta();
};

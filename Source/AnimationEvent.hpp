#pragma once

class Delegate;

class AnimationEvent
{
private:
	float time_;

	bool isInvoked_;

	Delegate * events_;

public:
	AnimationEvent();

	AnimationEvent(float);

	void Initialize(float);

	Delegate & GetEvents();

	float GetTime();

	void Invoke();

	void Reset();

	bool IsInvoked();
};

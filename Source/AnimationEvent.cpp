#include "AnimationEvent.hpp"

#include "Delegate.hpp"

AnimationEvent::AnimationEvent()
{
	events_ = nullptr;
}

AnimationEvent::AnimationEvent(float time)
{
	Initialize(time);
}

void AnimationEvent::Initialize(float time)
{
	time_ = time;

	events_ = new Delegate();
}

float AnimationEvent::GetTime()
{
	return time_;
}

Delegate & AnimationEvent::GetEvents()
{
	return *events_;
}

void AnimationEvent::Invoke()
{
	if(isInvoked_)
		return;

	events_->Invoke();

	isInvoked_ = true;
}

void AnimationEvent::Reset()
{
	isInvoked_ = false;
}

bool AnimationEvent::IsInvoked()
{
	return isInvoked_;
}

#pragma once

#include "Conventions.hpp"

class AnimationProperty;
class AnimationEvent;

class Animation
{
private:
	bool isPlaying_;

	bool willStop_;

	bool isLooping_;

	float time_;

	float length_;

	Array <AnimationProperty> properties_;

	Array <AnimationEvent> events_;

public:
	Animation();

	Animation(float);

	void Play();

	void Update();

	AnimationProperty* AddProperty();

	AnimationEvent* AddEvent();

	void AddProperty(AnimationProperty);

	float GetTime();

	float GetLength();
};

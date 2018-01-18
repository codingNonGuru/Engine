#pragma once

#include "Conventions.hpp"

class Animation;

class Animator
{
private:
	Map <Animation*, LongWord> animations_;

public:
	Animator();

	void Update();

	void Play(const char*);

	Animation* GetAnimation(const char*);

	Animation* AddAnimation(Animation*, const char*);
};
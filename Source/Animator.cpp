#include "Animator.hpp"

#include "Animation.hpp"

Animator::Animator()
{
	animations_.Initialize(8);
}

Animation* Animator::GetAnimation(const char* name)
{
	return *animations_.Get(name);
}

Animation* Animator::AddAnimation(Animation* animation, const char* name)
{
	auto animationPointer = animations_.Allocate(LongWord(name));
	if(!animationPointer)
		return nullptr;

	*animationPointer = animation;

	return *animationPointer;
}

void Animator::Update()
{
	for(auto animationIterator = animations_.GetStart(); animationIterator != animations_.GetEnd(); ++animationIterator)
	{
		auto animation = *animationIterator;
		if(!animation)
			return;

		animation->Update();
	}
}

void Animator::Play(const char* name)
{
	auto animation = animations_.Get(name);

	if(!animation)
		return;

	(*animation)->Play();
}


#include "CloseButton.hpp"

#include "Interface/Interface.hpp"
#include "Transform.hpp"
#include "Time.hpp"

void CloseButton::HandleConfigure()
{
	opacityFactor_ = 0.0f;
}

void CloseButton::HandleUpdate()
{
	if(!isInteractive_)
		return;

	if(isHovered_ && Interface::GetHoveredElement() == this)
	{
		opacityFactor_ += 0.1f;
	}

	//opacity_ = 0.5f + 0.5f * opacityFactor_;

	opacityFactor_ *= 0.8f;
}

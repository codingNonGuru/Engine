#include <stdlib.h>

#include "Element.hpp"
#include "Sprite.hpp"
#include "Transform.hpp"

Element::Element(Transform* transform, Sprite* sprite)
{
	Initialize(transform, sprite);
}

void Element::Initialize(Transform* transform, Sprite* sprite)
{
	transform_ = transform;

	parent_ = nullptr;

	sprite_ = sprite;
	sprite_->SetParent(this);
}

bool Element::IsActive()
{
	if(isActive_ == false)
		return false;
	else if(parent_ != nullptr)
		return parent_->IsActive();
	else
		return true;
}

void Element::Draw(Camera* camera)
{
	if(IsActive() == false)
		return;

	if(sprite_)
		sprite_->Draw(camera);
}

Position2 Element::GetLocalPosition()
{
	return transform_->position_;
}

Position2 Element::GetPosition()
{
	Position2 position = transform_->position_;

	if(parent_ == nullptr)
		return position;

	Element* parent = parent_;
	while(true)
	{
		if(parent)
		{
			Rotation rotation = parent->transform_->rotation_;
			float s = sin(rotation);
			float c = cos(rotation);

			float x = position.x * c - position.y * s;
			float y = position.x * s + position.y * c;

			position.x = parent->transform_->position_.x + x;
			position.y = parent->transform_->position_.y + y;

			parent = parent->parent_;
		}
		else
		{
			break;
		}
	}

	return position;
}

Rotation Element::GetRotation()
{
	Rotation rotation = transform_->rotation_;

	if(parent_ == nullptr)
		return rotation;

	Element* parent = parent_;
	while(true)
	{
		if(parent)
		{
			rotation += parent->transform_->rotation_;

			parent = parent->parent_;
		}
		else
		{
			break;
		}
	}

	return rotation;
}


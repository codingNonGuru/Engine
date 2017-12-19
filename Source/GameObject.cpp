#include "GameObject.hpp"

#include "Transform.hpp"

void GameObject::Enable()
{
	isActive_ = true;

	HandleEnable();
}

void GameObject::Disable()
{
	isActive_ = false;

	HandleDisable();
}

void GameObject::HandleEnable() {}

void GameObject::HandleDisable() {}

bool GameObject::IsLocallyActive()
{
	return isActive_;
}

bool GameObject::IsGloballyActive()
{
	bool isActive = isActive_;
	if(isActive == false)
		return false;

	GameObject* parent = parent_;

	while(true)
	{
		if(parent == nullptr)
			break;

		if(parent->IsLocallyActive() == false)
		{
			return false;
		}

		parent = parent->parent_;
	}

	return true;
}

Position3 GameObject::GetLocalPosition()
{
	return transform_->GetPosition();
}

Position3 GameObject::GetGlobalPosition()
{
	//if(!isAttached_)
		//return transform_->position_;

	GameObject* parent = parent_;
	Position3 position = transform_->GetPosition();

	while(true)
	{
		if(!parent)
			break;

		auto parentTransform = parent->GetTransform();

		float rotation = parentTransform->GetRotation();
		float s = sin(rotation);
		float c = cos(rotation);

		float x = position.x * c - position.y * s;
		float y = position.x * s + position.y * c;

		auto parentPosition = parentTransform->GetPosition();
		position.x = parentPosition.x + x;
		position.y = parentPosition.y + y;
		position.z = 0.0f;

		parent = parent->GetParent();
	}

	return position;
}

Rotation GameObject::GetLocalRotation()
{
	return transform_->GetRotation();
}

Rotation GameObject::GetGlobalRotation()
{
	Rotation rotation = transform_->GetRotation();

	if(parent_ == nullptr)
		return rotation;

	GameObject* parent = parent_;
	while(true)
	{
		if(parent)
		{
			rotation += parent->GetTransform()->GetRotation();

			parent = parent->GetParent();
		}
		else
		{
			break;
		}
	}

	return rotation;
}

GameObject* GameObject::GetParent()
{
	return parent_;
}

Transform* GameObject::GetTransform()
{
	return transform_;
}

void GameObject::SetParent(GameObject* parent)
{
	parent_ = parent;
}

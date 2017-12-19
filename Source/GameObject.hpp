#pragma once

#include "Conventions.hpp"

class Transform;

class GameObject
{
protected:
	bool isActive_;

	Transform* transform_;

	GameObject* parent_;

public:
	void Enable();

	void Disable();

	virtual void HandleEnable();

	virtual void HandleDisable();

	bool IsGloballyActive();

	bool IsLocallyActive();

	Position3 GetLocalPosition();

	Position3 GetGlobalPosition();

	Rotation GetLocalRotation();

	Rotation GetGlobalRotation();

	GameObject* GetParent();

	Transform* GetTransform();

	void SetParent(GameObject*);
};

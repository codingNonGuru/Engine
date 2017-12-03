#pragma once
#include <glm.hpp>

#include "Conventions.hpp"

class Sprite;
class Camera;
class Transform;

class Element
{
	bool isActive_;

	Transform* transform_;
	Sprite* sprite_;

	Element* parent_;
	Array <Element> children_;

public:
	Element(Transform*, Sprite*);
	void Initialize(Transform*, Sprite*);
	void Draw(Camera*);
	bool IsActive();
	Position2 GetPosition();
	Position2 GetLocalPosition();
	Rotation GetRotation();
};

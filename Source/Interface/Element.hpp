#pragma once

#include <glm/glm.hpp>

#include "Conventions.hpp"
#include "Types.hpp"
#include "GameObject.hpp"

class Sprite;
class Camera;
class Transform;
class Animator;
class AnimationProperty;
class Delegate;

class Element : public GameObject
{
protected:
	Sprite* sprite_;

	Animator* animator_;

	Array <GameObject*> children_;

	Size size_;

	DrawOrder drawOrder_;

	Opacity opacity_;

	bool isInteractive_;

	bool isHovered_;

	Delegate * clickEvents_;

	Delegate * hoverEvents_;

public:
	Element();

	Element(Size, Transform*, Sprite*);

	Element(Size, DrawOrder, Transform*, Sprite*, Opacity = 1.0f);

	void Initialize(Size, Transform*, Sprite*);

	void Initialize(Size, DrawOrder, Transform*, Sprite*, Opacity = 1.0f);

	bool CheckHover();

	void Update();

	virtual void Render(Camera*);

	Animator* GetAnimator();

	Size & GetSize();

	DrawOrder & GetDrawOrder();

	Opacity GetOpacity();

	Delegate & GetClickEvents();

	Delegate & GetHoverEvents();

	void SetInteractivity(bool);

	AnimationProperty* AddAnimationProperty(const char*, InterfaceElementParameters);

	void HandleClick();

	void HandleHover();

	void Open();

	void Close();

	virtual void HandleOpen();

	virtual void HandleClose();

	virtual void HandleEnable();

	virtual void HandleDisable();

	virtual void HandleInitialize() {}

	virtual void HandleUpdate() {}
};

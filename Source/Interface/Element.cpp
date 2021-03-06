#include <stdlib.h>

#include "Element.hpp"
#include "Sprite.hpp"
#include "Transform.hpp"
#include "Animation.hpp"
#include "Animator.hpp"
#include "AnimationEvent.hpp"
#include "AnimationProperty.hpp"
#include "InputHandler.hpp"
#include "Delegate.hpp"

#define DEFAULT_CHILDREN_COUNT 8

Element::Element()
{
	isActive_ = false;

	isInteractive_ = false;

	parent_ = nullptr;

	transform_ = nullptr;

	animator_ = nullptr;

	clickEvents_ = nullptr;

	HandleConfigure();
}

Element::Element(Size size, Transform* transform, Sprite* sprite)
{
	Configure(size, transform, sprite);
}

/*Element::Element(Size size, DrawOrder drawOrder, Transform* transform, Sprite* sprite)
{
	Initialize(size, drawOrder, transform, sprite);
}*/

Element::Element(Size size, DrawOrder drawOrder, Transform* transform, Sprite* sprite, Opacity opacity)
{
	Configure(size, drawOrder, transform, sprite, opacity);
}

void Element::Configure(Size size, Transform* transform, Sprite* sprite)
{
	isActive_ = false;

	isInteractive_ = false;

	transform_ = transform;

	parent_ = nullptr;

	animator_ = new Animator();

	sprite_ = sprite;
	sprite_->SetParent(this);

	size_ = size;

	drawOrder_ = 0;

	opacity_ = 1.0f;

	clickEvents_ = new Delegate();

	children_.Initialize(DEFAULT_CHILDREN_COUNT);

	HandleConfigure();
}

void Element::Configure(Size size, DrawOrder drawOrder, Transform* transform, Sprite* sprite, Opacity opacity)
{
	isActive_ = false;

	isInteractive_ = false;

	transform_ = transform;

	parent_ = nullptr;

	animator_ = new Animator();

	sprite_ = sprite;
	if(sprite_)
	{
		sprite_->SetParent(this);
	}

	size_ = size;

	drawOrder_ = drawOrder;

	opacity_ = opacity;

	clickEvents_ = new Delegate();

	children_.Initialize(DEFAULT_CHILDREN_COUNT);

	HandleConfigure();
}

Word Element::GetIdentifier()
{
	return identifier_;
}

void Element::SetIdentifier(Word identifier)
{
	identifier_ = identifier;
}

Sprite* Element::GetSprite()
{
	return sprite_;
}

void Element::Initialize()
{
	auto animation = animator_->GetAnimation("Close");
	if(animation != nullptr)
	{
		closeEvent_ = animation->GetFinishEvent();
		closeEvent_->GetActions().Add(this, &Element::Disable);
	}

	HandleInitialize();
}

bool Element::CheckHover()
{
	if(!isInteractive_)
		return false;

	if(IsGloballyActive() == false)
	{
		isHovered_ = false;
		return false;
	}

	auto mousePosition = InputHandler::GetMousePosition();

	auto position = GetGlobalPosition();

	bool isInsideHorizontally = mousePosition.x > position.x - size_.x / 2 && mousePosition.x < position.x + size_.x / 2;
	bool isInsideVertically = mousePosition.y > position.y - size_.y / 2 && mousePosition.y < position.y + size_.y / 2;

	isHovered_ = isInsideHorizontally && isInsideVertically;

	return isHovered_;
}

void Element::Update()
{
	if(IsGloballyActive() == false)
		return;

	if(animator_)
	{
		animator_->Update();
	}

	HandleUpdate();
}

void Element::Render(Camera* camera)
{
	if(IsGloballyActive() == false)
		return;

	if(!sprite_)
		return;

	sprite_->Draw(camera);
}

AnimationProperty* Element::AddAnimationProperty(const char* animationName, InterfaceElementParameters parameter)
{
	auto animation = animator_->GetAnimation(animationName);
	if(!animation)
		return nullptr;

	AnimationProperty * property = nullptr;
	switch(parameter)
	{
	case InterfaceElementParameters::POSITION_X:
		property = animation->AddProperty();
		property->Initialize(&transform_->GetPosition().x);
		return property;
	case InterfaceElementParameters::POSITION_Y:
		property = animation->AddProperty();
		property->Initialize(&transform_->GetPosition().y);
		return property;
	case InterfaceElementParameters::OPACITY:
		property = animation->AddProperty();
		property->Initialize(&opacity_);
		return property;
	}

	return nullptr;
}

Size & Element::GetSize()
{
	return size_;
}

DrawOrder & Element::GetDrawOrder()
{
	return drawOrder_;
}

Opacity Element::GetOpacity()
{
	return opacity_;
}

Delegate & Element::GetClickEvents()
{
	return *clickEvents_;
}

Delegate & Element::GetHoverEvents()
{
	return *hoverEvents_;
}

Animator* Element::GetAnimator()
{
	return animator_;
}

void Element::SetInteractivity(bool isInteractive)
{
	isInteractive_ = isInteractive;
}

void Element::HandleClick()
{
	if(!clickEvents_)
		return;

	clickEvents_->Invoke();
}

void Element::Open()
{
	if(!animator_ || isActive_)
		return;

	isActive_ = true;

	animator_->Play("Open");

	HandleOpen();
}

void Element::Close()
{
	if(!animator_ || !isActive_)
		return;

	animator_->Play("Close");

	HandleClose();
}

void Element::AddChild(Element* child)
{
	auto childPointer = children_.Allocate();
	if(childPointer == nullptr)
		return;

	*childPointer = child;
}

Element* Element::GetChild(Word identifier)
{
	for(auto childIterator = children_.GetStart(); childIterator != children_.GetEnd(); ++childIterator)
	{
		auto child = *childIterator;
		if(child == nullptr)
			continue;

		if(child->GetIdentifier() == identifier)
			return child;
	}

	return nullptr;
}

void Element::HandleOpen()
{
}

void Element::HandleClose()
{
}

void Element::HandleEnable()
{
}

void Element::HandleDisable()
{

}

void Element::HandleSetParent(Object* parent)
{
	auto parentAsElement = (Element*)parent;
	parentAsElement->AddChild(this);
}

void Element::HandleConfigure() {}

void Element::HandleInitialize() {}

void Element::HandleUpdate() {}

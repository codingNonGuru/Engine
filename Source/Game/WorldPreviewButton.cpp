#include "Interface/Interface.hpp"
#include "TextureManager.hpp"
#include "Interface/Sprite.hpp"
#include "Delegate.hpp"

#include "WorldPreviewButton.hpp"
#include "Game/WorldPreviewPanel.hpp"

void WorldPreviewButton::HandleInitialize()
{
	previewPanel_ = (WorldPreviewPanel*)Interface::GetElement("WorldPreview");

	SetInteractivity(true);
	clickEvents_->Add(this, &WorldPreviewButton::Press);

	auto icon = GetChild("Icon");
	if(icon != nullptr)
	{
		auto texture = TextureManager::GetTexture("ReliefIcon");
		auto iconSprite = icon->GetSprite();

		iconSprite->SetTexture(texture);
	}
}

void WorldPreviewButton::SetMode(WorldPreviewModes mode)
{
	mode_ = mode;
}

void WorldPreviewButton::Press()
{
	if(previewPanel_ != nullptr)
	{
		previewPanel_->SetViewMode(mode_);
	}
}

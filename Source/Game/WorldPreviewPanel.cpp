#include "Interface/Interface.hpp"
#include "Interface/Sprite.hpp"
#include "Animator.hpp"
#include "Animation.hpp"
#include "AnimationEvent.hpp"

#include "Game/WorldPreviewPanel.hpp"
#include "Game/WorldGenerator.hpp"

void WorldPreviewPanel::HandleInitialize()
{
	previewImage_ = GetChild("PreviewImage");

	WorldGenerator::OnWorldGenerated_.Add(this, &WorldPreviewPanel::RefreshImage);
}

void WorldPreviewPanel::RefreshImage()
{
	SetViewMode(WorldPreviewModes::RELIEF);
}

void WorldPreviewPanel::SetViewMode(WorldPreviewModes mode)
{
	auto sprite = previewImage_->GetSprite();
	Texture* texture = nullptr;

	switch(mode)
	{
	case WorldPreviewModes::RELIEF:
		texture = WorldGenerator::GetReliefPreview();
		break;
	case WorldPreviewModes::BIOME:
		texture = WorldGenerator::GetBiomePreview();
		break;
	case WorldPreviewModes::POLITY:
		texture = WorldGenerator::GetCulturePreview();
		break;
	}

	sprite->SetTexture(texture);
}


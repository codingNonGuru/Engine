#include "Interface/Interface.hpp"
#include "Interface/Sprite.hpp"

#include "WorldPreviewPanel.hpp"
#include "Game/WorldGenerator.hpp"

void WorldPreviewPanel::HandleInitialize()
{
	previewImage_ = GetChild("PreviewImage");

	WorldGenerator::OnWorldGenerated_.Add(this, &WorldPreviewPanel::RefreshImage);
}

void WorldPreviewPanel::RefreshImage()
{
	auto sprite = previewImage_->GetSprite();
	auto texture = WorldGenerator::GetReliefPreview();

	sprite->SetTexture(texture);
}


#pragma once

#include "Interface/Element.hpp"

#include "Game/Types.hpp"

class WorldPreviewPanel : public Element
{
	Element* previewImage_;

	virtual void HandleInitialize() override;

public:
	void RefreshImage();

	void SetViewMode(WorldPreviewModes);
};

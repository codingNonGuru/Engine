#pragma once

#include "Interface/Element.hpp"

#include "Game/Types.hpp"

class WorldPreviewPanel;

class WorldPreviewButton : public Element
{
	WorldPreviewModes mode_;

	WorldPreviewPanel* previewPanel_;

	virtual void HandleInitialize() override;

public:
	void SetMode(WorldPreviewModes);

	void Press();
};

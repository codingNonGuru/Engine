#pragma once

#include "Interface/Element.hpp"

class WorldPreviewPanel : public Element
{
	Element* previewImage_;

	virtual void HandleInitialize() override;

public:
	void RefreshImage();
};

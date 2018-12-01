#pragma once

#include "Interface/Element.hpp"

class Text;

class BottomInfoPanel : public Element
{
	Text* populationLabel_;

	Text* buildingLabel_;

	Text* developmentLabel_;

protected:
	virtual void HandleInitialize() override;

	virtual void HandleUpdate() override;
};

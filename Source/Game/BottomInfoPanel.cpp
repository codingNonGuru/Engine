#include "Interface/Text.hpp"

#include "Game/BottomInfoPanel.hpp"
#include "Game/WorldScene.hpp"
#include "Game/World.hpp"
#include "Game/Settlement.hpp"
#include "Game/SettlementRenderer.hpp"
#include "Game/Economy.hpp"

void BottomInfoPanel::HandleInitialize()
{
	populationLabel_ = (Text*)GetChild("PopulationLabel");

	buildingLabel_ = (Text*)GetChild("BuildingLabel");

	developmentLabel_ = (Text*)GetChild("DevelopmentLabel");
}

Word populationString;
Word buildingString;
Word developmentString;

void BottomInfoPanel::HandleUpdate()
{
	if(populationLabel_ == nullptr)
		return;

	auto selection = WorldScene::GetSelectedObject();
	if(selection->Object_ == nullptr)
		return;

	auto settlement = (Settlement*)selection->Object_;

	auto population = settlement->GetPopulation();

	populationString = Word("Population: ");
	sprintf(populationString.GetEnd(), "%d", population);
	populationLabel_->Setup(populationString, 0.7f);

	auto buildingCount = settlement->GetRenderData()->BuildingCount_;

	buildingString = Word("Buildings: ");
	sprintf(buildingString.GetEnd(), "%d", buildingCount);
	buildingLabel_->Setup(buildingString, 0.5f);

	const Economy& economy = settlement->GetEconomy();

	developmentString = Word("Development: ");
	sprintf(developmentString.GetEnd(), "%f", economy.GetDevelopment());
	developmentLabel_->Setup(developmentString, 0.5f);
}

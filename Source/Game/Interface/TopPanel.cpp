#include "Interface/Text.hpp"
#include "Interface/Interface.hpp"
#include "Interface/Font.hpp"
#include "Interface/FontManager.hpp"
#include "Transform.hpp"

#include "Game/Interface/TopPanel.hpp"
#include "Game/InterfaceBuilder.hpp"
#include "Game/World.hpp"
#include "Game/WorldScene.hpp"
#include "Game/Economy.hpp"

void TopPanel::HandleInitialize()
{
	auto font = FontManager::GetFont("Dominican");
	auto textColor = InterfaceBuilder::GetTextColor();

	developmentLabel_ = Interface::AddElement("PopulationLabel", new Text(font, textColor));
	developmentLabel_->Configure(Size(256, 256), DrawOrder(2), new Transform(Position2(0.0f, 0.0f)), nullptr);

	developmentLabel_->Enable();
	developmentLabel_->SetParent(this);

	potentialLabel_ = Interface::AddElement("PopulationLabel", new Text(font, textColor));
	potentialLabel_->Configure(Size(256, 256), DrawOrder(2), new Transform(Position2(0.0f, 50.0f)), nullptr);

	potentialLabel_->Enable();
	potentialLabel_->SetParent(this);
}

static Word developmentString;
static Word potentialString;

void TopPanel::HandleUpdate()
{
	world_ = WorldScene::GetWorld();
	auto & economies = world_->GetEconomies();

	float development = 0.0f;
	float potential = 0.0f;
	for(auto economy = economies.GetStart(); economy != economies.GetEnd(); ++economy)
	{
		development += economy->GetDevelopment();
		potential += economy->GetPotential();
	}

	developmentString = Word("Development: ");
	sprintf(developmentString .GetEnd(), "%d", int(development * 100.0f));
	developmentLabel_->Setup(developmentString, 0.5f);

	potentialString = Word("Potential: ");
	sprintf(potentialString .GetEnd(), "%d", int(potential * 100.0f));
	potentialLabel_->Setup(potentialString, 0.5f);
}

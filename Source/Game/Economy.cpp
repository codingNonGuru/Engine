#include "Utility/Utility.hpp"

#include "Economy.hpp"
#include "Game/Settlement.hpp"
#include "Game/World.hpp"
#include "Game/Tile.hpp"

#define BASE_DEVELOPMENT_RATE 0.01f

#define BASE_DECAY_RATE 0.001f

Economy::Economy() {}

Economy::Economy(Settlement* settlement) : settlement_(settlement)
{
	float productivity = 0.0f;

	auto & tiles = Settlement::world_->GetTiles();
	for(int x = settlement_->longitude_ - 1; x <= settlement_->longitude_ + 1; ++x)
	{
		for(int y = settlement_->latitude_ - 1; y <= settlement_->latitude_ + 1; ++y)
		{
			auto tile = tiles(x, y);
			auto & biome = tile->GetBiome();

			productivity += biome.Productivity_;
		}
	}

	float tonnesPerAcre = 0.15f;

	float effectiveAcres = productivity * 250.0f;

	float aggregateYield = tonnesPerAcre * effectiveAcres;

	float tonnesPerPerson = 0.3f;

	settlement_->population_ = aggregateYield / tonnesPerPerson;

	resource_ = productivity;

	technology_ = utility::GetRandom(1.0f, 3.0f);

	float potentialDevelopment = resource_ * technology_;

	development_ = potentialDevelopment * utility::GetRandom(0.2f, 0.5f);
}

void Economy::Update()
{
	float potentialDevelopment = resource_ * technology_;

	if(development_ > potentialDevelopment)
	{
		float developmentDelta = potentialDevelopment - development_;
		float overshootModifier = exp(-pow(developmentDelta, 2.0f) / pow(potentialDevelopment * 0.1f, 2.0f));
		development_ *= 1.0f + BASE_DEVELOPMENT_RATE * overshootModifier;
	}
	else
	{
		development_ *= 1.0f + BASE_DEVELOPMENT_RATE;
	}

	development_ *= 1.0f - BASE_DECAY_RATE;
}

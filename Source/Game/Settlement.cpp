#include "Game/Settlement.hpp"
#include "Game/World.hpp"
#include "Game/Tile.hpp"

World* Settlement::world_ = nullptr;

void Settlement::SetWorld(World* world)
{
	world_ = world;
}

void Settlement::Initialize(Position2 position)
{
	position_ = position;

	auto & tiles = world_->GetTiles();
	tile_ = tiles(position_.x, position_.y);

	longitude_ = (int)position_.x;
	latitude_ = (int)position_.y;

	EvaluateFertility();
}

int Settlement::GetPopulation() const
{
	return population_;
}

void Settlement::EvaluateFertility()
{
	float productivity = 0.0f;

	auto & tiles = world_->GetTiles();
	for(int x = longitude_ - 1; x <= longitude_ + 1; ++x)
	{
		for(int y = latitude_ - 1; y <= latitude_ + 1; ++y)
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

	population_ = aggregateYield / tonnesPerPerson;

	//std::cout<<productivity<<" "<<aggregateYield<<" "<<population_<<"\n";
}

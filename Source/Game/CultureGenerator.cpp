#include "Utility/Utility.hpp"

#include "CultureGenerator.hpp"
#include "Game/World.hpp"
#include "Game/Settlement.hpp"
#include "Game/Tile.hpp"

#define MAXIMUM_SETTLEMENT_COUNT 65536

void CultureGenerator::Generate(World& world)
{
	auto & tiles = world.GetTiles();

	auto & settlements = world.GetSettlements();

	settlements.Initialize(MAXIMUM_SETTLEMENT_COUNT);

	Settlement::SetWorld(&world);

	int globalPopulation = 0;
	int highestPopulation = 0;
	int lowestPopulation = 65536;
	for(int x = 0; x < tiles.GetWidth(); ++x)
	{
		for(int y = 0; y < tiles.GetHeight(); ++y)
		{
			auto tile = tiles(x, y);
			auto & biome = tile->GetBiome();

			float chance = biome.Productivity_;
			if(chance < 0.1f)
				continue;

			bool hasFound = false;
			int range = 2;
			for(int i = x - range; i <= x + range; ++i)
			{
				for(int j = y - range; j <= y + range; ++j)
				{
					if(tiles(i, j)->GetSettlement() != nullptr)
					{
						hasFound = true;
						break;
					}
				}
			}

			if(hasFound == true)
				continue;

			chance = utility::Interpolate(chance);
			chance = pow(chance, 8.0f);

			if(utility::RollDice(chance) == false)
				continue;

			auto settlement = settlements.Allocate();
			if(settlement == nullptr)
				continue;

			Position2 position = Position2(x, y) + Position2(utility::GetRandom(0.2f, 0.8f), utility::GetRandom(0.2f, 0.8f));
			settlement->Initialize(position);

			tile->SetSettlement(settlement);

			globalPopulation += settlement->GetPopulation();

			if(settlement->GetPopulation() > highestPopulation)
				highestPopulation = settlement->GetPopulation();

			if(settlement->GetPopulation() < lowestPopulation)
				lowestPopulation = settlement->GetPopulation();
		}
	}

	std::cout<<"SETTLEMENT COUNT ----------> "<<settlements.GetSize()<<"\n";
	std::cout<<"GLOBAL POPULATION ----------> "<<globalPopulation<<"\n";
	std::cout<<"HIGHEST POPULATION ----------> "<<highestPopulation<<"\n";
	std::cout<<"LOWEST POPULATION ----------> "<<lowestPopulation<<"\n";
	std::cout<<"AVERAGE POPULATION ----------> "<<globalPopulation / settlements.GetSize()<<"\n";
}

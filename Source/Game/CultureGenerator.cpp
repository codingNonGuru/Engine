#include "Utility/Utility.hpp"
#include "Container/Block.hpp"
#include "Texture.hpp"
#include "Time.hpp"

#include "CultureGenerator.hpp"
#include "Game/World.hpp"
#include "Game/WorldGenerator.hpp"
#include "Game/Settlement.hpp"
#include "Game/Tile.hpp"

#define MAXIMUM_SETTLEMENT_COUNT 65536

void CultureGenerator::Generate(World& world)
{
	GenerateSettlements(world);

	GenerateRoads(world);

	GenerateDomains(world);

	auto & tiles = world.GetTiles();

	auto size = world.GetSize();
	Grid <Byte4> textureData(size.x, size.y);
	for(int x = 0; x < size.x; ++x)
	{
		for(int y = 0; y < size.y; ++y)
		{
			auto tile = tiles(x, y);
			if(tile->GetRelief() == ReliefTypes::OCEAN)
			{
				*textureData(x, y) = Byte4(128, 0, 0, 0);
			}
			/*else
			{
				if(tile->GetSettlement() != nullptr)
				{
					*textureData(x, y) = Byte4(255, 255, 255, 255);
				}
				else
				{
					*textureData(x, y) = Byte4(128, 0, 0, 255);
				}
			}*/
			else
			{
				auto domain = tile->GetDomain();
				if(domain != nullptr)
				{
					auto banner = domain->GetBanner();
					*textureData(x, y) = Byte4(banner.r_ * 255.0f, 0, 0, 255);
				}
				else
				{
					*textureData(x, y) = Byte4(128, 0, 0, 255);
				}
			}
		}
	}

	auto texture = WorldGenerator::GetCulturePreview();
	texture->Upload(textureData.GetStart());
}

void CultureGenerator::GenerateSettlements(World& world)
{
	Time::StartClock();

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

			chance *= 0.05f;

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

	auto time = Time::GetClock();
	std::cout<<"SETTLEMENT GENERATION TIME (in seconds) ------> "<<time<<"\n";

	std::cout<<"SETTLEMENT COUNT ----------> "<<settlements.GetSize()<<"\n";
	std::cout<<"GLOBAL POPULATION ----------> "<<globalPopulation<<"\n";
	std::cout<<"HIGHEST POPULATION ----------> "<<highestPopulation<<"\n";
	std::cout<<"LOWEST POPULATION ----------> "<<lowestPopulation<<"\n";
	std::cout<<"AVERAGE POPULATION ----------> "<<globalPopulation / settlements.GetSize()<<"\n";
}

struct LinkData
{
	Settlement* Settlement_;

	container::Block <Settlement*, 16> Links_;

	Index Count_;

	LinkData()
	{
		for(auto link = Links_.GetStart(); link != Links_.GetEnd(); ++link)
			*link = nullptr;

		Count_ = 0;
	}

	void AddLink(Settlement* settlement)
	{
		*Links_.Find(Count_) = settlement;

		Count_++;
	}

	bool HasLink(Settlement* settlement)
	{
		for(auto link = Links_.GetStart(); link != Links_.GetStart() + Count_; ++link)
		{
			if(settlement == *link)
				return true;
		}

		return false;
	}
};

#define DEFAULT_MINIMUM_DISTANCE 100.0f

#define MAXIMUM_DISTANCE 15.0f

#define MINIMUM_ANGLE 3.1415f / 6.0f

#define ROAD_GENERATION_PASS_COUNT 4

void CultureGenerator::GenerateRoads(World& world)
{
	auto & settlements = world.GetSettlements();
	auto & tiles = world.GetTiles();

	Array <LinkData> linkDatas(settlements.GetSize());
	linkDatas.AllocateFully();

	int searchRange = 15;

	for(int pass = 0; pass < ROAD_GENERATION_PASS_COUNT; ++pass)
	{
		auto linkData = linkDatas.GetStart();
		for(auto settlement = settlements.GetStart(); settlement != settlements.GetEnd(); ++settlement, ++linkData)
		{
			float minimumDistance = DEFAULT_MINIMUM_DISTANCE;
			Settlement* closestSettlement = nullptr;

			auto position = settlement->GetPosition();
			for(int x = (int)position.x - searchRange; x <= (int)position.x + searchRange; ++x)
			{
				for(int y = (int)position.y - searchRange; y <= (int)position.y + searchRange; ++y)
				{
					auto tile = tiles(x, y);
					auto otherSettlement = tile->GetSettlement();
					if(otherSettlement == nullptr || otherSettlement == settlement)
						continue;

					float distance = settlement->GetDistance(otherSettlement);
					if(distance > minimumDistance || distance > MAXIMUM_DISTANCE)
						continue;

					minimumDistance = distance;
					closestSettlement = otherSettlement;
				}
			}

			if(closestSettlement == nullptr)
				continue;

			if(linkData->HasLink(closestSettlement) == true)
				continue;

			bool isTooNarrow = false;

			auto directionToClosest = closestSettlement->GetPosition() - settlement->GetPosition();
			directionToClosest = glm::normalize(directionToClosest);
			for(auto link = linkData->Links_.GetStart(); link != linkData->Links_.GetStart() + linkData->Count_; ++link)
			{
				auto linkedSettlement = *link;
				if(linkedSettlement == nullptr)
					continue;

				auto directionToLinked = linkedSettlement->GetPosition() - settlement->GetPosition();
				directionToLinked = glm::normalize(directionToLinked);

				auto angle = glm::dot(directionToLinked, directionToClosest);
				angle = acos(angle);

				if(angle > MINIMUM_ANGLE)
					continue;

				isTooNarrow = true;
				break;
			}

			if(isTooNarrow == true)
				continue;

			auto otherSettlementIndex = closestSettlement - settlements.GetStart();
			auto otherLinkData = linkDatas.GetStart() + otherSettlementIndex;

			linkData->AddLink(closestSettlement);
			otherLinkData->AddLink(settlement);
		}
	}

	int linkCount = 0;
	for(auto linkData = linkDatas.GetStart(); linkData != linkDatas.GetEnd(); ++linkData)
	{
		linkCount += linkData->Count_;
	}
	std::cout<<"TOTAL NUMBER OF LINKS BETWEEN SETTLEMENTS --------> "<<linkCount<<"\n";
}

void CultureGenerator::GenerateDomains(World& world)
{
	Time::StartClock();

	auto & tiles = world.GetTiles();

	auto & settlements = world.GetSettlements();

	int searchRange = 20;

	for(auto tile = tiles.GetStart(); tile != tiles.GetEnd(); ++tile)
	{
		if(tile->GetRelief() == ReliefTypes::OCEAN)
			continue;

		float minimumDistance = DEFAULT_MINIMUM_DISTANCE;
		Settlement* closestSettlement = nullptr;

		auto basePosition = tile->GetPosition();
		for(int x = (int)basePosition.x - searchRange; x <= (int)basePosition.x + searchRange; ++x)
		{
			for(int y = (int)basePosition.y - searchRange; y <= (int)basePosition.y + searchRange; ++y)
			{
				auto otherTile = tiles(x, y);

				auto otherSettlement = otherTile->GetSettlement();
				if(otherSettlement == nullptr)
					continue;

				float distance = glm::length(otherTile->GetPosition() - tile->GetPosition());
				if(distance > minimumDistance)
					continue;

				minimumDistance = distance;
				closestSettlement = otherSettlement;
			}
		}

		if(closestSettlement == nullptr)
			continue;

		tile->SetDomain(closestSettlement);
	}

	auto time = Time::GetClock();
	std::cout<<"DOMAIN GENERATION TIME ------> "<<time<<" seconds\n";
}

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
			else
			{
				if(tile->GetSettlement() != nullptr)
				{
					*textureData(x, y) = Byte4(255, 255, 255, 255);
				}
				else
				{
					*textureData(x, y) = Byte4(128, 0, 0, 255);
				}
			}
			/*else
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
			}*/
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
			if(tile->GetLandRatio() < 0.1f)
				continue;

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

			auto key = settlement - settlements.GetStart();
			settlement->SetKey(key);

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

	container::Block <Settlement*, 64> Links_;

	container::Block <Settlement*, 64> Blocks_;

	Length LinkCount_;

	Length BlockCount_;

	LinkData()
	{
		for(auto link = Links_.GetStart(); link != Links_.GetEnd(); ++link)
			*link = nullptr;

		LinkCount_ = 0;

		for(auto block = Blocks_.GetStart(); block != Blocks_.GetEnd(); ++block)
			*block = nullptr;

		BlockCount_ = 0;
	}

	void AddLink(Settlement* settlement)
	{
		*Links_.Find(LinkCount_) = settlement;

		LinkCount_++;
	}

	void BlockLink(Settlement* settlement)
	{
		*Blocks_.Find(BlockCount_) = settlement;

		BlockCount_++;
	}

	bool HasLink(Settlement* settlement)
	{
		for(auto block = Blocks_.GetStart(); block != Blocks_.GetStart() + BlockCount_; ++block)
		{
			if(settlement == *block)
				return true;
		}

		for(auto link = Links_.GetStart(); link != Links_.GetStart() + LinkCount_; ++link)
		{
			if(settlement == *link)
				return true;
		}

		return false;
	}
};

#define DEFAULT_MINIMUM_DISTANCE 1000.0f

#define MAXIMUM_DISTANCE 20.0f

#define MINIMUM_ANGLE 3.1415f / 6.0f

#define ROAD_GENERATION_PASS_COUNT 24

bool IsTooNarrow(Settlement* thisSettlement, Settlement* otherSettlement, LinkData* thisLinkData, LinkData* otherLinkData)
{
	auto directionToClosest = otherSettlement->GetPosition() - thisSettlement->GetPosition();
	directionToClosest = glm::normalize(directionToClosest);
	for(auto link = thisLinkData->Links_.GetStart(); link != thisLinkData->Links_.GetStart() + thisLinkData->LinkCount_; ++link)
	{
		auto linkedSettlement = *link;
		if(linkedSettlement == nullptr)
			continue;

		auto directionToLinked = linkedSettlement->GetPosition() - thisSettlement->GetPosition();
		directionToLinked = glm::normalize(directionToLinked);

		auto angle = glm::dot(directionToLinked, directionToClosest);
		angle = acos(angle);

		if(angle > MINIMUM_ANGLE)
			continue;

		thisLinkData->BlockLink(otherSettlement);
		otherLinkData->BlockLink(thisSettlement);

		return true;
	}

	return false;
}

void CultureGenerator::GenerateRoads(World& world)
{
	auto & settlements = world.GetSettlements();
	auto & tiles = world.GetTiles();

	Array <LinkData> linkDatas(settlements.GetSize());
	linkDatas.AllocateFully();

	int searchRange = 21;
	Length linkCount = 0;
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

					if(linkData->HasLink(otherSettlement))
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

			auto otherLinkData = linkDatas.GetStart() + closestSettlement->GetKey();

			bool isTooNarrow = IsTooNarrow(settlement, closestSettlement, linkData, otherLinkData);
			if(isTooNarrow)
				continue;

			isTooNarrow = IsTooNarrow(closestSettlement, settlement, otherLinkData, linkData);
			if(isTooNarrow)
				continue;

			linkData->AddLink(closestSettlement);
			otherLinkData->AddLink(settlement);

			linkCount += 2;
		}
		std::cout<<"LINKS "<<linkCount<<"\n";
	}

	auto & links = world.GetLinks();
	links.Initialize(linkCount);

	auto linkData = linkDatas.GetStart();
	for(auto settlement = settlements.GetStart(); settlement != settlements.GetEnd(); ++settlement, ++linkData)
	{
		auto firstLink = links.GetEnd();
		settlement->SetLinkNetwork(LinkNetwork(firstLink, linkData->LinkCount_));
		for(auto linkIterator = linkData->Links_.GetStart(); linkIterator != linkData->Links_.GetStart() + linkData->LinkCount_; ++linkIterator)
		{
			auto link = *linkIterator;
			*links.Allocate() = Link(link);
		}
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

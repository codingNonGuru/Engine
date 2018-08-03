#include "Tile.hpp"

Tile::Tile()
{
	settlement_ = nullptr;

	domain_ = nullptr;
}

Position3 & Tile::GetPosition()
{
	return position_;
}

ReliefTypes Tile::GetRelief()
{
	return landRatio_ > 0.5f ? ReliefTypes::LAND : ReliefTypes::OCEAN;
}

Float Tile::GetLandRatio()
{
	return landRatio_;
}

void Tile::SetLandRatio(Float landRatio)
{
	landRatio_ = landRatio;
}

Biome & Tile::GetBiome()
{
	return biome_;
}

Settlement* Tile::GetSettlement()
{
	return settlement_;
}

void Tile::SetSettlement(Settlement* settlement)
{
	settlement_ = settlement;
}

Settlement* Tile::GetDomain()
{
	return domain_;
}

void Tile::SetDomain(Settlement* domain)
{
	domain_ = domain;
}

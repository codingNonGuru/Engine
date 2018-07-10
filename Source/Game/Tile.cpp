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

ReliefTypes & Tile::GetRelief()
{
	return relief_;
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

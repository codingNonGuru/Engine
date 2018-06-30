#include "Tile.hpp"

Position3 & Tile::GetPosition()
{
	return position_;
}

ReliefTypes & Tile::GetRelief()
{
	return relief_;
}

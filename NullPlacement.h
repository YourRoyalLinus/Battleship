#pragma once
#include "ShipPlacementStrategy.h"
class NullPlacement : public ShipPlacementStrategy
{
public:
	NullPlacement(Player& player) : ShipPlacementStrategy(player){}
	void place() override {}
};

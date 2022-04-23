#pragma once
#include "ShipPlacementStrategy.h"
class HumanShipPlacement : public ShipPlacementStrategy
{
public:
	HumanShipPlacement(Player& player) : ShipPlacementStrategy(player) {}
	void place() override;
};


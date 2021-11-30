#pragma once
#include "ShipPlacementStrategy.h"
class ComputerShipPlacement : public ShipPlacementStrategy
{
public:
	ComputerShipPlacement(Player& computer) : ShipPlacementStrategy(computer) {}
	void place() override;

};


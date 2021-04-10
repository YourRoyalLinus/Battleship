#pragma once
#include <vector>
#include "Ship.h"
class Player
{
public:
	std::vector<Ship> ships = { Ship(Ship::Type::CARRIER), Ship(Ship::Type::BATTLESHIP), Ship(Ship::Type::CRUISER), Ship(Ship::Type::SUBMARINE), Ship(Ship::Type::DESTROYER) };
};


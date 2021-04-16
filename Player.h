#pragma once
#include <vector>
#include "Board.h"
#include "Ship.h"

class Player
{
	public:
		Player() {
			board = new Board(Board::Type::PLAYER);
		}

		~Player() {
			delete board;
		}

		Board* board;
		std::vector<Ship> ships = { Ship(Ship::Type::CARRIER), Ship(Ship::Type::BATTLESHIP), Ship(Ship::Type::CRUISER), Ship(Ship::Type::SUBMARINE), Ship(Ship::Type::DESTROYER) };
};


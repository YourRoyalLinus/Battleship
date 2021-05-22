#pragma once

#include "Board.h"


class Opponent {
	public:
		virtual ~Opponent() {
			delete board;
		}

		static void DeallocOpponent(Opponent* o) {
			delete o;
		}

		virtual std::pair<int, int> GuessCoordinate() = 0;
		virtual void SankShip() = 0;

		std::pair<int, int> lastHitSqaure;
		bool hitStreak = false;
		Board* board;
		std::vector<Ship> ships = { Ship(Ship::Type::CARRIER), Ship(Ship::Type::BATTLESHIP), Ship(Ship::Type::CRUISER), Ship(Ship::Type::SUBMARINE), Ship(Ship::Type::DESTROYER) }; 

	
};


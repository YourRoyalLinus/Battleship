#pragma once
#include "GameParams.h"
#include "Board.h"
#include "GuessStrategy.h"
#include <functional>

class Player{
	public:
		enum class Type {
			HERO,
			HUMAN_OPPONENT,
			EASY_COMPUTER,
			MEDIUM_COMPUTER,
			HARD_COMPUTER
		};

		Player(Type type);

		void guess(Player& opponent) {
			guessStrategy->guess(*this, opponent);
		}
		void placeShip() { shipPlacementProc(); }


		Board* board;
		std::vector<Ship> ships = { Ship(Ship::Type::CARRIER), Ship(Ship::Type::BATTLESHIP), Ship(Ship::Type::CRUISER), Ship(Ship::Type::SUBMARINE), Ship(Ship::Type::DESTROYER) };
		GuessStrategy* guessStrategy;
		Ship* shipToPlace;

	private:
		std::function<void()> shipPlacementProc;
		
};


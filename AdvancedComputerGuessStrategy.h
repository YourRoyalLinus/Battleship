#pragma once
#include "ComputerGuessStrategy.h"

class AdvancedComputerGuessStrategy : public ComputerGuessStrategy {
	public:
		virtual void guess(Player& player, Player& opponent) = 0;
	
	protected:
		const std::vector<std::pair<int, int>> adjacentGuesses = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };
		std::vector<std::pair<int, int>> hitSquares;
		std::pair<int, int> prevGuess = { -1, -1 };
		std::pair<int, int> lastHitSquare = { -1, -1 };

		bool seekingShip = false;
		bool hitStreak = false;
		int adjGuessIx = -1;
		
	
};
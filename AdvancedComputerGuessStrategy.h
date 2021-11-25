#pragma once
#include "ComputerGuessStrategy.h"
class AdvancedComputerGuessStrategy {
	public:
		virtual void guess(Player& player, Player& opponent) = 0;
	
	protected:
		const std::vector<std::pair<int, int>> adjacentGuesses = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };
		std::set<std::pair<int, int>> guesses;
		std::pair<int, int> prevGuess = { -1, -1 };
		std::pair<int, int> lastHitSqaure = { -1, -1 };

		bool hitStreak = false;
		int adjGuessIx = -1;
		int backtrack = 0;
	
	
};
#pragma once
#include <set>
#include <vector>
#include "GuessStrategy.h"



class ComputerGuessStrategy : public GuessStrategy{
	public:
		virtual void guess(Player& player, Player& opponent) = 0;

	protected:
		//Advanced calculations variables
		int adjGuessIx = -1;
		std::pair<int, int> prevGuess;
		std::set<std::pair<int, int>> guesses;
		const std::vector<std::pair<int, int>> adjacentGuesses = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };
		std::pair<int, int> lastHitSqaure;
	
};
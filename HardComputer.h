#pragma once
#include "ComputerGuessStrategy.h"

class HardComputer : public ComputerGuessStrategy
{
	public:
		void guess(Player& player, Player& opponent) override;

private:
	bool hitStreak = false;
	int adjGuessIx = -1;
	int backtrack = 0;
	const std::vector<std::pair<int, int>> adjacentGuesses = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };
	std::pair<int, int> lastHitSqaure;
};



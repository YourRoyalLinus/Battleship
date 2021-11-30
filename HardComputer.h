#pragma once
#include "AdvancedComputerGuessStrategy.h"

class HardComputer : public AdvancedComputerGuessStrategy
{
	public:
		void guess(Player& player, Player& opponent) override;
	private:
		std::pair<int, int> GenerateRandomValidGuess();
		std::pair<int, int> ContinueOnDirection();
		std::pair<int, int> Backtrack(int& backtrack);

		void Reset();
		void ValidateGuess(std::pair<int, int>& guess);

		bool IsFirstHit();
		bool IsWithinBoundry(int row, int column);
		bool SetNextDirection();
		bool SubmitGuess(std::pair<int, int> guess, Player& opponent);	
};



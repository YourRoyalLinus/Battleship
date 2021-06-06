#pragma once
#include "ComputerGuessStrategy.h"

class HardComputer : public ComputerGuessStrategy
{
	public:
		void guess(Player& player, Player& opponent) override;

private:
	bool hitStreak;
};



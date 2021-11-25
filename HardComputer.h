#pragma once
#include "AdvancedComputerGuessStrategy.h"

class HardComputer : public AdvancedComputerGuessStrategy
{
	public:
		void guess(Player& player, Player& opponent) override;
	private:
};



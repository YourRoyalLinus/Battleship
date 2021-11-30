#pragma once
#include "ComputerGuessStrategy.h"

class EasyComputer : public ComputerGuessStrategy
{
	public:
		void guess(Player& player, Player& opponent) override;
};



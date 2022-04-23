#pragma once
#include "ComputerGuessStrategy.h"

class MediumComputer : public ComputerGuessStrategy
{
	void guess(Player& player, Player& opponent) override;
};


#pragma once
#include "GuessStrategy.h"
class SinglePlayerGuess : public GuessStrategy
{
	void guess(Player& player, Player& opponent) override;
};


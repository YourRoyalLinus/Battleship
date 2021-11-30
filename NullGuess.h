#pragma once
#include "GuessStrategy.h"
class NullGuess : public GuessStrategy
{
	void guess(Player& player, Player& opponent) override {}
};


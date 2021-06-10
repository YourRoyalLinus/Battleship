#pragma once
#include "GuessStrategy.h"
class NullStrategy : public GuessStrategy
{
	void guess(Player& player, Player& opponent) override {}
};


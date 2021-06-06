#pragma once
#include "GuessStrategy.h"
class HumanGuessStrategy : public GuessStrategy
{
	void guess(Player& player, Player& opponent) override;
};


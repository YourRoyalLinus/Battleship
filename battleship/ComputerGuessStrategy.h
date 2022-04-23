#pragma once
#include <set>
#include <vector>
#include "GuessStrategy.h"



class ComputerGuessStrategy : public GuessStrategy{
	public:
		virtual void guess(Player& player, Player& opponent) = 0;
	protected:
		std::set<std::pair<int, int>> guesses;
};
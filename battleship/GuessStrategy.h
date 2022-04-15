#pragma once
#include <utility>

class Player;
class GuessStrategy {
public:
	virtual void guess(Player& player, Player& opponent) = 0;
};

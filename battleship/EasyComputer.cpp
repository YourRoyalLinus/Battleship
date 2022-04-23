#pragma once
#include "EasyComputer.h"
#include "Player.h"

void EasyComputer::guess(Player& player, Player& opponent) {
	int row = rand() % 8;
	int col = rand() % 8;
	std::pair<int, int> guess = { row, col };
	opponent.board->guess(guess);
}


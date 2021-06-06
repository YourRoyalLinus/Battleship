#pragma once
#include "MediumComputer.h"
#include "Player.h"


void MediumComputer::guess(Player& player, Player& opponent) {
	int row = rand() % 8;
	int col = rand() % 8;
	std::pair<int, int> guess = { row, col };
	while (guesses.size() < 64 && guesses.find(guess) != guesses.end()) {
		int row = rand() % 8;
		int col = rand() % 8;
		guess = { row, col };
	}
	guesses.insert(guess);
	opponent.board->guess(guess);
}
#include "EasyComputer.h"

EasyComputer::EasyComputer(const Difficulty& d) {
	difficulty = d;
	board = new Board(Board::Type::RADER);
}

void EasyComputer::SankShip() {
	hitStreak = false;
	adjGuessIx = -1;
}

std::pair<int, int> EasyComputer::GuessCoordinate() {
	int row = rand() % 8;
	int col = rand() % 8;
	std::pair<int, int> guess = { row, col };
	return guess;
}


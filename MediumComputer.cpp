#include "MediumComputer.h"

MediumComputer::MediumComputer(const Difficulty& d) {
	difficulty = d;
	board = new Board(Board::Type::RADER);
}

void MediumComputer::SankShip() {
	hitStreak = false;
	adjGuessIx = -1;
}

std::pair<int, int> MediumComputer::GuessCoordinate() {
	int row = rand() % 8;
	int col = rand() % 8;
	std::pair<int, int> guess = { row, col };
	while (guesses.size() < 64 && guesses.find(guess) != guesses.end()) {
		int row = rand() % 8;
		int col = rand() % 8;
		guess = { row, col };
	}

	guesses.insert(guess);

	return guess;
}
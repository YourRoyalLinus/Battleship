#pragma once
#include "HardComputer.h"
#include "Player.h"


void HardComputer::guess(Player& player, Player& opponent){
	std::pair<int, int> dir;
	int row = rand() % 8;
	int col = rand() % 8;
	std::pair<int, int> guess = { row, col };
	if (hitStreak) lastHitSqaure = prevGuess;

	if(adjGuessIx > 3){
		adjGuessIx = -1;
	}

	if (hitStreak && adjGuessIx == -1){
		if ((lastHitSqaure.first + 1) > 7) {
			adjGuessIx += 2;
		}
		else {
			adjGuessIx++;
		}

		dir = adjacentGuesses[adjGuessIx];
		row = lastHitSqaure.first + dir.first;
		col = lastHitSqaure.second + dir.second;

		guess = { row, col };
	}
	else if (hitStreak && adjGuessIx > -1) {
		if (adjGuessIx == 0 && (lastHitSqaure.first + 1) > 7) {
			adjGuessIx++;
		}
		if (adjGuessIx == 1 && (lastHitSqaure.first - 1) < 0) {
			adjGuessIx++;
		}
		if (adjGuessIx == 2 && (lastHitSqaure.second + 1) > 7) {
			adjGuessIx++;
		}

		dir = adjacentGuesses[adjGuessIx];
		row = lastHitSqaure.first + dir.first;
		col = lastHitSqaure.second + dir.second;

		guess = { row, col };
	}
	else if (hitStreak && adjGuessIx > -1) {
		if (adjGuessIx == 0 && (lastHitSqaure.first - 1) < 0) {
			adjGuessIx++;
		}
		if (adjGuessIx == 1 && (lastHitSqaure.second + 1) > 7) {
			adjGuessIx++;
		}
		adjGuessIx++;

		dir = adjacentGuesses[adjGuessIx];
		row = lastHitSqaure.first + dir.first;
		col = lastHitSqaure.second + dir.second;
		guess = { row, col };
	} 
	
	int backtrack = 0;
	while (guesses.size() < 64 && guesses.find(guess) != guesses.end()) {
		if (adjGuessIx == 1) {
			backtrack--;
			dir = adjacentGuesses[adjGuessIx];
			row = lastHitSqaure.first + dir.first + backtrack;
			col = lastHitSqaure.second + dir.second;

			guess = { row, col };
		}
		else if (adjGuessIx == 3) {
			backtrack--;
			dir = adjacentGuesses[adjGuessIx];
			row = lastHitSqaure.first + dir.first;
			col = lastHitSqaure.second + dir.second + backtrack;

			guess = { row, col };
		}
		else {
			row = rand() % 8;
			col = rand() % 8;
			guess = { row, col };
		}

	}

	guesses.insert(guess);
	prevGuess = guess;

	hitStreak = opponent.board->guess(guess);
}
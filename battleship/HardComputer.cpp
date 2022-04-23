#pragma once
#include "HardComputer.h"
#include "Player.h"


void HardComputer::guess(Player& player, Player& opponent){
	std::pair<int, int> guess;

	if (opponent.board->checkShipSank()) {
		Reset();
	}
	else if (hitStreak) {
		hitSquares.push_back(prevGuess);
		lastHitSquare = prevGuess;
		seekingShip = true;
	}
	
	if (hitStreak && IsFirstHit()){
		if (SetNextDirection()) {
			guess = ContinueOnDirection();
			ValidateGuess(guess);
			hitStreak = SubmitGuess(guess, opponent);
		}
		else {
			guess = GenerateRandomValidGuess();
			hitStreak = SubmitGuess(guess, opponent);
		}
	}
	else if (seekingShip && hitStreak) {
		guess = ContinueOnDirection();
		ValidateGuess(guess);
		hitStreak = SubmitGuess(guess, opponent);
	}
	else if (seekingShip && !hitStreak) {
		if (adjGuessIx != 2) {
			if (SetNextDirection()) {
				guess = ContinueOnDirection();
				ValidateGuess(guess);
				hitStreak = SubmitGuess(guess, opponent);
			}
			else {
				guess = GenerateRandomValidGuess();
				hitStreak = SubmitGuess(guess, opponent);
			}
		}
		else {
			int backtrack = 0;
			guess = Backtrack(backtrack);
			hitStreak = SubmitGuess(guess, opponent);
		}
	}
	else {
		guess = GenerateRandomValidGuess();
		hitStreak = SubmitGuess(guess, opponent);
	}
}

std::pair<int, int> HardComputer::GenerateRandomValidGuess() {
	const int maxPossibleGuesses = 8 * 8; //Rows X Columns;
	if (guesses.size() == maxPossibleGuesses) {
		std::cout << "GAME OVER: ALL GUESSES HAVE BEEN MADE." << std::endl; //TODO MAKE GRAPHIC
		exit(-1);
	}

	std::pair<int, int> guess = { (rand() % 8),  (rand() % 8) };
	while (guesses.find(guess) != guesses.end()) {
		guess.first = rand() % 8;
		guess.second = rand() % 8;
	}

	Reset();
	return guess;
}

std::pair<int, int> HardComputer::ContinueOnDirection() {
	std::pair<int, int> dir = adjacentGuesses[adjGuessIx];
	int row = lastHitSquare.first + dir.first;
	int col = lastHitSquare.second + dir.second;

	return { row, col };
}

std::pair<int, int> HardComputer::Backtrack(int& backtrack) {
	std::pair<int, int> dir;
	int row;
	int col;
	std::pair<int, int> guess = prevGuess;
	while (guesses.find(guess) != guesses.end()) {
		backtrack--;
		if (adjGuessIx == 1) {
			dir = adjacentGuesses[adjGuessIx];
			row = lastHitSquare.first + dir.first + backtrack;
			col = lastHitSquare.second + dir.second;

			guess = { row, col };
		}
		else if (adjGuessIx == 2) {
			dir = adjacentGuesses[adjGuessIx];
			row = lastHitSquare.first + dir.first;
			col = lastHitSquare.second + dir.second + backtrack;

			guess = { row, col };
		}

	}

	return guess;
}

void HardComputer::ValidateGuess(std::pair<int, int>& guess) {
	if(!IsWithinBoundry(guess.first, guess.second)) {
		if (SetNextDirection()) {
			guess = ContinueOnDirection();
		}
		else {
			guess = GenerateRandomValidGuess();
		}
	}

	 int backtrack = 0;
	while (guesses.find(guess) != guesses.end()) {
		if (std::find(hitSquares.begin(), hitSquares.end(), guess) != hitSquares.end()) {
			guess = Backtrack(backtrack);
		}
		else if (SetNextDirection()) {
			guess = ContinueOnDirection();
		}
		else if (adjGuessIx == 1 || adjGuessIx == 2) {
			guess = Backtrack(backtrack);
			
		}		
		else{
			guess = GenerateRandomValidGuess();
		}
	}
}

void HardComputer::Reset() {
	seekingShip = false;
	hitStreak = false;
	adjGuessIx = -1;
}

bool HardComputer::IsFirstHit() {
	return (adjGuessIx == -1);
}

bool HardComputer::IsWithinBoundry(int row, int column) {
	if (row < 0 || column < 0 || row > 7 || column > 7) {
		return false;
	}
	else {
		return true;
	}
}


bool HardComputer::SetNextDirection() {
	if (adjGuessIx == -1 && (lastHitSquare.first + 1) > 7) {
		adjGuessIx++;
	}
	if (adjGuessIx == 0 && (lastHitSquare.first - 1) < 0) {
		adjGuessIx++;
	}
	if (adjGuessIx == 1 && (lastHitSquare.second + 1) > 7) {
		adjGuessIx++;
	}
	if (adjGuessIx == 2 && (lastHitSquare.second - 1) < 0) {
		adjGuessIx++;
	}

	adjGuessIx++;

	if (adjGuessIx > 3) {
		return false;
	}
	else {
		return true;
	}

}

bool HardComputer::SubmitGuess(std::pair<int, int> guess, Player& opponent) {
	guesses.insert(guess);
	prevGuess = guess;

	return opponent.board->guess(guess);
}


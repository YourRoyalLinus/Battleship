#pragma once
#include "ExpertComputer.h"
#include "Player.h"


void ExpertComputer::guess(Player& player, Player& opponent) {
	const int lastRow = 8;
	const int lastColumn = 8;
	const int firstRow = 0;
	const int firstColumn = 0;

	std::pair<int, int> dir;
	int row = rand() % 8;
	int col = rand() % 8;
	std::pair<int, int> guess = { row, col };

	if (_openStrat->isOpeningPhase) {
		_openStrat->guess(player, opponent);
		prevGuess = _openStrat->GetPrevGuess();
		guesses.insert(prevGuess);
		if(_openStrat->isTransitionaryTurn) {
			hitStreak = true;
			lastHitSquare = prevGuess;
		} 
	}
	else if (opponent.board->checkShipSank()) {
		hitStreak = 0;
		adjGuessIx = -1;
		//PH
		//PH
		if (!possibleShips.empty()) {
			possibleShips.pop(); //Get rid of first hitMark as its to the base ship (will always be sunk)
			std::pair<int, int> firstHit = possibleShips.front();
			std::pair<int, int> lastHit = prevGuess;

			if (firstHit.first == lastHit.first || firstHit.second == lastHit.second) {
				std::pair<int, int> possibleNewShip = possibleShips.front();
				possibleShips.pop();

				if ((possibleNewShip.first + 1) > 7) {
					adjGuessIx += 2;
				}
				else {
					adjGuessIx++;
				}

				dir = adjacentGuesses[adjGuessIx];
				row = possibleNewShip.first + dir.first;
				col = possibleNewShip.second + dir.second;

				guess = { row, col };

				if (adjGuessIx != -1) {
					while (guesses.find(guess) != guesses.end() && adjGuessIx < 4) {
						if (adjGuessIx == 0 && (lastHitSquare.first + 1) < 7) {
							adjGuessIx++;
						}
						if (adjGuessIx == 1 && (lastHitSquare.second + 1) < 7) {
							adjGuessIx++;
						}
						if (adjGuessIx == 2 && (lastHitSquare.first - 1) > 0) {
							adjGuessIx++;
						}

						dir = adjacentGuesses[adjGuessIx];
						row = lastHitSquare.first + dir.first;
						col = lastHitSquare.second + dir.second;

						guess = { row, col };
					}
				}

				if (adjGuessIx > 3) {
					hitStreak = 0;
					adjGuessIx = -1;
					//PH
					//PH

					while (guesses.find(guess) != guesses.end()) {
						int row = rand() % 8;
						int col = rand() % 8;
						guess = { row, col };
					}
				}
			}
			else {
				while (!possibleShips.empty()) {
					possibleShips.pop();
				}
			}
		}
		else {
			while (guesses.find(guess) != guesses.end()) {
				int row = rand() % 8;
				int col = rand() % 8;
				guess = { row, col };
			}
		}

		guesses.insert(guess);
		prevGuess = guess;

		hitStreak = opponent.board->guess(guess);
	}
	else{
		if (hitStreak) {
			lastHitSquare = prevGuess;
			possibleShips.push(prevGuess);
		}

		if (adjGuessIx > 3) {
			adjGuessIx = -1;
		}

		if (hitStreak && adjGuessIx == -1) {
			if ((lastHitSquare.first + 1) > 7) {
				adjGuessIx += 2;
			}
			else {
				adjGuessIx++;
			}

			dir = adjacentGuesses[adjGuessIx];
			row = lastHitSquare.first + dir.first;
			col = lastHitSquare.second + dir.second;

			guess = { row, col };
		}
		else if (hitStreak && adjGuessIx > -1) { //Changes ported to Hard Computer
			if (adjGuessIx == 0 && (lastHitSquare.first + 1) > 7) {
				adjGuessIx+=2;
			}
			if (adjGuessIx == 1 && (lastHitSquare.second + 1) > 7) {
				adjGuessIx+=2;
			}
			if (adjGuessIx == 2 && (lastHitSquare.first - 1) < 0) {
				adjGuessIx+=2;
			}

			dir = adjacentGuesses[adjGuessIx];
			row = lastHitSquare.first + dir.first;
			col = lastHitSquare.second + dir.second;

			guess = { row, col };
		}
		else if (!hitStreak && adjGuessIx > -1) {
			if (adjGuessIx == 0 && (lastHitSquare.first - 1) < 0) {
				adjGuessIx++;
			}
			if (adjGuessIx == 1 && (lastHitSquare.second + 1) > 7) {
				adjGuessIx++;
			}

			adjGuessIx++;

			dir = adjacentGuesses[adjGuessIx];
			row = lastHitSquare.first + dir.first;
			col = lastHitSquare.second + dir.second;
			guess = { row, col };
		}
		
		if (adjGuessIx != -1) {
			while (guesses.find(guess) != guesses.end() && adjGuessIx < 4) {
				if (adjGuessIx == 0 && (lastHitSquare.first + 1) < 7) {
					adjGuessIx++;
				}
				if (adjGuessIx == 1 && (lastHitSquare.second + 1) < 7) {
					adjGuessIx++;
				}
				if (adjGuessIx == 2 && (lastHitSquare.first - 1) > 0) {
					adjGuessIx++;
				}

				dir = adjacentGuesses[adjGuessIx];
				row = lastHitSquare.first + dir.first;
				col = lastHitSquare.second + dir.second;

				guess = { row, col };
			}
		}

		if (adjGuessIx > 3) {
			hitStreak = 0;
			adjGuessIx = -1;
			//PH
			//PH

			while (guesses.find(guess) != guesses.end()) {
				int row = rand() % 8;
				int col = rand() % 8;
				guess = { row, col };
			}
		}

		int backtrack = 0;
		while (guesses.find(guess) != guesses.end()) {
			if (adjGuessIx == 1) {
				backtrack--;
				dir = adjacentGuesses[adjGuessIx];
				row = lastHitSquare.first + dir.first + backtrack;
				col = lastHitSquare.second + dir.second;

				guess = { row, col };
			}
			else if (adjGuessIx == 3) {
				backtrack--;
				dir = adjacentGuesses[adjGuessIx];
				row = lastHitSquare.first + dir.first;
				col = lastHitSquare.second + dir.second + backtrack;

				guess = { row, col };
			}
			else {
				int row = rand() % 8;
				int col = rand() % 8;
				guess = { row, col };
			}

		}

		guesses.insert(guess);
		prevGuess = guess;

		hitStreak = opponent.board->guess(guess);
	}
}
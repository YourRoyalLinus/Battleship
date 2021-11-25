#include "DiagonalOpeningStrategy.h"
#include "Player.h"

void DiagonalOpeningStrategy::guess(Player& player, Player& opponent) {
	const int lastRow = 7;
	const int lastColumn = 7;
	const int firstRow = 0;
	const int firstColumn = 0;

	std::pair<int, int> dir;
	std::pair<int, int> guess;
	
	if (isOpeningPhase) { 
		if (prevGuess.first == -1 && prevGuess.second == -1) {
			isTransitionaryTurn = (opponent.board->guess({ 0 , 0 }));
			isOpeningPhase = !isTransitionaryTurn;

			prevGuess = { 0 , 0 };
		}
		else {
			if (!mainDiagonalChecked) {
				if (prevGuess.first == lastRow && prevGuess.second == lastColumn) {
					mainDiagonalChecked = true;
					guess.first = 0;
					guess.second = 1;

					isTransitionaryTurn = opponent.board->guess(guess);
					isOpeningPhase = !isTransitionaryTurn;

					prevGuess = guess;
					return;
				}
				else {
					guess.first = ++prevGuess.first;
					guess.second = ++prevGuess.second;

					prevGuess = guess;

					isTransitionaryTurn = opponent.board->guess(guess);
					isOpeningPhase = !isTransitionaryTurn;
					return;
				}
			}
			if (!topDiagonalChecked) {
				if (prevGuess.second == lastColumn) {
					if (prevGuess.first != firstRow) {
						guess.first = 0;
						guess.second = (8 - prevGuess.first);

						prevGuess = guess;

						isTransitionaryTurn = opponent.board->guess(guess);
						isOpeningPhase = !isTransitionaryTurn;
						return;
					}
					else {
						topDiagonalChecked = true;
						guess.first = 1;
						guess.second = 0;

						isTransitionaryTurn = opponent.board->guess(guess);
						isOpeningPhase = !isTransitionaryTurn;

						prevGuess = guess;
						return;
					}
				}
				else {
					guess.first = ++prevGuess.first;
					guess.second = ++prevGuess.second;


					prevGuess = guess;

					isTransitionaryTurn = opponent.board->guess(guess);
					isOpeningPhase = !isTransitionaryTurn;
					return;
				}
			}
			if (!bottomDiagonalChecked) {
				if (prevGuess.first == lastRow) {
					if (prevGuess.second != firstColumn) {
						guess.first = (8 - prevGuess.second);
						guess.second = 0;

						prevGuess = guess;

						isTransitionaryTurn = opponent.board->guess(guess);
						isOpeningPhase = !isTransitionaryTurn;
						return;
					}
					else {
						bottomDiagonalChecked = true;
						guess.first = -1;
						guess.second = -1;
					}
				}
				else {
					guess.first = ++prevGuess.first;
					guess.second = ++prevGuess.second;

					prevGuess = guess;

					isTransitionaryTurn = opponent.board->guess(guess);
					isOpeningPhase = !isTransitionaryTurn;
					return;
				}
			}
		}
	}
}


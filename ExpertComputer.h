#pragma once
#include "ComputerGuessStrategy.h"
#include "OpeningStrategy.h"
#include "DiagonalOpeningStrategy.h"
#include <queue>

class ExpertComputer : public ComputerGuessStrategy
{
	public:
		ExpertComputer() {
			_openStrat = new DiagonalOpeningStrategy();
		}
		ExpertComputer(OpeningStrategy* strat) {
			_openStrat = strat;
		}
		~ExpertComputer() {
			delete _openStrat;
			_openStrat = 0;
		}
		void guess(Player& player, Player& opponent) override;
	protected:
		OpeningStrategy* _openStrat;
	private:
		bool hitStreak = false;
		int consecutiveHits = 0;
		int adjGuessIx = -1; //REMOVE COMPUTER DOUBLE-GUESSING
		int backtrack = 0;
		const std::vector<std::pair<int, int>> adjacentGuesses = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} }; //If you ever strike more than 1 time in one direction, and 0 times in the opposite direction, you've hit more than 1 ship
		std::pair<int, int> lastHitSquare;
		std::queue<std::pair<int, int>> possibleShips;
};


//Keep a list of all Hit Squares until we sink a ship. If we sink a ship on a row/column orthogonal to the square we started at, and we hit more than 1 Square, we've hit more than 1 ship
//We Keep a list of possibleDiffShipCoords and clear if we sink a ship on a row/column parallel to where we started
//If we're orthogonal to where we started, go back and pop the first possibleDiffShipCoord and set the index to the first adjacenetIx orthogonal to that square
//WE NEED TO BE ALERTED WHEN WE'VE SUNK A SHIP (LOGICAL) WE NEED TO CHECK FOR possibleDiffShipCoords immediately after sinking a ship


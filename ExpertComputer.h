#pragma once
#include "AdvancedComputerGuessStrategy.h"
#include "DiagonalOpeningStrategy.h"
#include <stack>

class ExpertComputer : public AdvancedComputerGuessStrategy
{
	public:
		ExpertComputer() {
			openingStrategy = new DiagonalOpeningStrategy();
		}
		ExpertComputer(OpeningStrategy* strat) {
			openingStrategy = strat;
		}
		~ExpertComputer() {
			delete openingStrategy;
			openingStrategy = 0;
		}
		void guess(Player& player, Player& opponent) override;
	protected:
		OpeningStrategy* openingStrategy;
	private:
		int hitsBetweenSinkingShips = 0;
		bool adjacentShipFound = false;
		std::stack<std::pair<int, int>> possibleShipStartCoords;

		std::pair<int, int> GenerateRandomValidGuess();
		std::pair<int, int> ContinueOnDirection();
		std::pair<int, int> Backtrack(int& backtrack);

		void Reset();
		void ValidateGuess(std::pair<int, int>& guess);
		

		bool IsFirstHit();
		bool SetNextDirection();
		bool IsPossibleAdjacentShipCoord();
		bool IsWithinBoundry(int row, int column);

		bool SubmitGuess(std::pair<int, int> guess, Player& opponent);
		
};


//Keep a list of all Hit Squares until we sink a ship. If we sink a ship on a row/column orthogonal to the square we started at, and we hit more than 1 Square, we've hit more than 1 ship
//We Keep a list of possibleDiffShipCoords and clear if we sink a ship on a row/column parallel to where we started
//If we're orthogonal to where we started, go back and pop the first possibleDiffShipCoord and set the index to the first adjacenetIx orthogonal to that square
//WE NEED TO BE ALERTED WHEN WE'VE SUNK A SHIP (LOGICAL) WE NEED TO CHECK FOR possibleDiffShipCoords immediately after sinking a ship


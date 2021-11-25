#pragma once
#include "ComputerGuessStrategy.h"

class OpeningStrategy : public ComputerGuessStrategy {
	public:
		void guess(Player& player, Player& opponent) override {
		}
		bool isOpeningPhase = true;
		bool isTransitionaryTurn = false;
		std::pair<int, int> GetPrevGuess(){
			return prevGuess;
		}
	
};
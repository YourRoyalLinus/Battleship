#pragma once
#include "Computer.h"

class EasyComputer : public Computer
{
	public:
		//Computer board which looks like a radar board to the player
		EasyComputer(const Difficulty& d);

		std::pair<int, int> GuessCoordinate();

		void SankShip();
};



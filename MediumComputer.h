#pragma once
#include "Computer.h"

class MediumComputer : public Computer
{
	public:
		//Computer board which looks like a radar board to the player
		MediumComputer(const Difficulty& d);

		std::pair<int, int> GuessCoordinate();

		void SankShip();
};


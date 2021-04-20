#pragma once
#include "Computer.h"

class HardComputer : public Computer
{
	public:
		HardComputer(const Difficulty& d);

		std::pair<int, int> GuessCoordinate();

		void SankShip();
};



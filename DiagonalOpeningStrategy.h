#pragma once
#include "OpeningStrategy.h"

class DiagonalOpeningStrategy : public OpeningStrategy
{
	public:
		void guess(Player& player, Player& opponent) override;
	private:
		bool topDiagonalChecked = false;
		bool bottomDiagonalChecked = false;
		bool mainDiagonalChecked = false;
};


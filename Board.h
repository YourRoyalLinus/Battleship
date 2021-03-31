#pragma once
#include <vector>
#include <iostream>
#include "Square.h"
#include "Ship.h"


class Board {

public:
	static const int BOARD_WIDTH = 8;
	static const int BOARD_HEIGHT = 8;

	Board();
	void print();
	/* Try to place a piece. Return whether or not it was a valid placement */
	bool Board::placeShip(Ship& ship, std::vector<std::pair<int, int>> coords);
	std::vector<Square> occupiedSquares();



private:
	std::vector<std::vector<Square>> squares;
	std::vector<Ship> activeShips;
	
};

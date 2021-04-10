#pragma once
#include <iostream>
#include "Ship.h"

class Square {
	friend std::ostream& operator<<(std::ostream& os, Square& square);
public:
	Square(int x, int y, bool occupied = false) : row(x), col(y), occupied(occupied), type(Ship::Type::EMPTY) {}
	bool occupied;
	Ship::Type type;
	int row;
	int col;

private:
	
};

std::ostream& operator<<(std::ostream& os, Square& square);
	

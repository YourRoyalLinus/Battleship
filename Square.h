#pragma once
#include <iostream>
#include "Ship.h"

class Square {
	friend std::ostream& operator<<(std::ostream& os, Square& square);
public:
	//Square() = default;
	Square(int row, int col, bool occupied = false) : x(row), y(col), occupied(occupied), type(Ship::Type::EMPTY) {}
	bool occupied;
	Ship::Type type;

private:
	int x;
	int y;

};

std::ostream& operator<<(std::ostream& os, Square& square);
	

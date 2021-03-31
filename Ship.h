#pragma once
#include <vector>
//#include "Square.h"

class Ship {
public:
	enum class Type {
		CARRIER = 5,
		BATTLESHIP = 4,
		CRUISER = 3,
		SUBMARINE = 3,
		DESTROYER = 3,
		EMPTY = 0
	};
	Ship(Type type): type(type) {}
	Type type;
	//std::vector<Square> occupying;
};





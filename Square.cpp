#pragma once
#include "Square.h"
#include <iostream>

std::ostream& operator<<(std::ostream& os, Square& square) {
	os << (int)square.type;
	return os;
}

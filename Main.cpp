//#include <iostream>
#include "Board.h"


int main() {
	//Make a board
	Board board;
	board.print();

	//Put a ship down on it.
	Ship ship(Ship::Type::BATTLESHIP);
	using Coord = std::pair<int, int>;
	Coord c1(4, 4);
	Coord c2(4, 5);
	Coord c3(4, 6);
	Coord c4(4, 7);
	std::vector<Coord> coords = { c1,c2,c3,c4 };
	board.placeShip(ship, coords);

	std::cout << "\n\n\n\n" << std::endl;

	board.print();
	
	

	std::cin.get();
	
}
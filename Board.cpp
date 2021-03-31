#include "Board.h"

/*Initialize the squares on the board with their Coordinates and being unoccupied*/
Board::Board() {
	std::vector<Square> currentRow;
	for (int row = 0; row < BOARD_WIDTH; row++) {
		currentRow.clear();
		for (int col = 0; col < BOARD_HEIGHT; col++)
			currentRow.push_back(Square(row, col));
		squares.push_back(currentRow);
	}
}

void Board::print() {
	for (auto row : squares) {
		for (auto sq : row)
			std::cout << sq << " ";
		std::cout << std::endl;
	}
}

/* To try and place a ship, specify the coords you want to place it at. The first int in a coordinate pair corresponds to the row, the second to the column.
   If any of the coordinates go off the board or are already occupied, return false. Otherwise place the ship and return true. */
bool Board::placeShip(Ship& ship, std::vector<std::pair<int,int>> coords) {

	//TODO: The coordinate validation and checking for occupied might need to be abstracted out later for other uses!

	int row = -1;
	int col = -1;

	/* First, check if the coords are valid */
	for (auto coord : coords) {
		/* If this is the first coord you are looking at keep track of what row/col you are on.*/
		if (row == -1) {
			row = coord.first;
			col = coord.second;
		}

		if (coord.first >= BOARD_WIDTH || coord.second >= BOARD_HEIGHT 
			|| coord.first < 0 || coord.second < 0 
			|| (coord.first != row && coord.second != col) /* Because you can't place a piece diagonally, atleast 1 of row or col need to be the same the whole time */)
			return false;
	}

	/* Now check if they are already occupied */
	for (auto coord : coords) {
		Square& square = squares[coord.first][coord.second];
		if (square.occupied)
			return false;
		else {
			//ship.occupying.push_back(square);
			square.occupied = true;
			square.type = ship.type;
		}
	}

	return true;
}

std::vector<Square> Board::occupiedSquares() {
	std::vector<Square> result;
	for (auto row : squares) {
		for (auto square : row) {
			if (square.occupied)
				result.push_back(square);
		}
	}
	return result;
}
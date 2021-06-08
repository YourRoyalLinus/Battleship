#pragma once
#include <algorithm>
#include "ResourceManager.h"
#include "Board.h"
#include "PlayerHitEvent.h"


const glm::vec2 Board::PLAYER_BOARD_POSITION = glm::vec2(600.0f, 0.0f);
const glm::vec2 Board::RADAR_BOARD_POSITION = glm::vec2(0.0f, 0.0f);
const glm::vec2 Board::SIZE = glm::vec2(600.0f, 600.0f);

//TODO: THIS CONSTRUCTOR IS A FUCKING MESS 
Board::Board(Type type) : type(type), Entity(type == Type::WATER ? PLAYER_BOARD_POSITION : RADAR_BOARD_POSITION, SIZE, ResourceManager::getTexture( type == Type::WATER ? "water" : "radar")) {
	//TODO: CLEAN THIS UP
	if (type == Type::WATER) {
		waveMap = ResourceManager::getTexture("waveMap");
	}
	else if (type == Type::RADER) {
		waveMap = ResourceManager::getTexture("pings");
	}
	/*Initialize the squares on the board with their Coordinates and being unoccupied*/
	std::vector<Square> currentRow;
	for (int row = 0; row < BOARD_WIDTH; row++) {
		currentRow.clear();
		for (int col = 0; col < BOARD_HEIGHT; col++)
			currentRow.push_back(Square(row, col));
		squares.push_back(currentRow);
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

		if (!validCoord(coord) || (coord.first != row && coord.second != col)) { // A ship can't be placed diagonally, so either the row or the col needs to stay the same the whole time
			return false;
		}

	}

	/* Now check if they are already occupied */
	for (auto coord : coords) {
		Square& square = squares[coord.first][coord.second];
		if (square.occupied)
			return false;
	}
	/* If none of them were occupied place the ship*/
	for (auto coord : coords) {
		Square& square = squares[coord.first][coord.second];
		ship.coords.push_back(coord);
		square.occupied = true;
		square.type = ship.type;
	}
	

	activeShips.push_back(ship);

	return true;
}

void Board::print() {
	for (auto row : squares) {
		for (auto square : row) {
			std::cout << (square.occupied ? "1" : "0") << " ";
		}
		std::cout << std::endl;
	}

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


bool Board::guess(const std::pair<int, int> coord) {
	bool validSquare = validCoord(coord);
	//If you tried to guess at an invalid position
	if (!validSquare)
		return false;

	Square& square = squares[coord.first][coord.second];
	bool hit = square.occupied;
	//If the square has already been guessed return false.
	for (auto start = guessedSquares.begin(); start != guessedSquares.end(); ++start) {
		if (start->row == square.row && start->col == square.col) {
			hit = false;
		}
	}
	//TODO this is really ugly and can probably be done a better way! Should the board even be doing this?
	if (hit) {
		//tell observers that a player's ship was hit!
		this->notify(new PlayerHitEvent(coord));
		damageHitShip(coord);
	}
	guessedSquares.push_back(square);
	return hit;
}

bool Board::alreadyGuessedSquare(const std::pair<int, int> coord) {
	for (auto sq : guessedSquares) {
		if (sq.row == coord.first && sq.col == coord.second) {
			return true;
		}
	}
	return false;
}

void Board::markSquare(const std::pair<int, int> coord, bool prevRes) {
	Square& sq = squares[coord.first][coord.second];
	if (prevRes) {
		sq.occupied = true;
	}
	else {
		sq.occupied = false;
	}
	guessedSquares.push_back(sq);
}

bool Board::validCoord(const std::pair<int, int> coord) {
	return coord.first < BOARD_WIDTH && coord.second < BOARD_HEIGHT && coord.first >= 0 && coord.second >= 0;
}

void Board::damageHitShip(std::pair<int,int> coord) {
	for (auto currentShip = activeShips.begin(); currentShip != activeShips.end(); currentShip++) {
		auto hitSquare = std::find(currentShip->coords.begin(), currentShip->coords.end(), coord);
		if (hitSquare != currentShip->coords.end()) //if the ship contains the coord guess
		{

			currentShip->hitsTaken++;
			if (currentShip->sunk()) {
				for (auto coord : currentShip->coords) {
					squares[coord.first][coord.second].occupied = false;
				}
				//damageSankShip(*currentShip);
				currentShip = activeShips.erase(currentShip);
				break;
			}
		}
	}
}

//void Board::damageSankShip(Ship ship) {
//	std::string shipType;
//	std::string sinker;
//	std::string sinkee;
//
//	switch (ship.type) {
//	case Ship::Type::BATTLESHIP:
//		shipType = "BattleShip";
//		break;
//	case Ship::Type::CARRIER:
//		shipType = "Carrier";
//		break;
//	case Ship::Type::CRUISER:
//		shipType = "Cruiser";
//		break;
//	case Ship::Type::DESTROYER:
//		shipType = "Destroyer";
//		break;
//	case Ship::Type::SUBMARINE:
//		shipType = "Submarine";
//		break;
//	}
//	if (turn == GameParams::Turn::PLAYER) {
//		sinker = "Player";
//		sinkee = "Opponent";
//	}
//	else {
//		sinker = "Opponent";
//		sinkee = "Player";
//	}
//
//	std::cout << sinker << " has sunk " << sinkee << "'s " << shipType << "!" << std::endl;
//}

void Board::draw(SpriteRenderer& spriteRenderer) {
	spriteRenderer.DrawSprite(this->sprite, this->position, this->size, this->rotation, this->color, glm::vec2(0), this->waveMap);
}

Board::~Board() {

}
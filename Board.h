#pragma once
#include <vector>
#include <iostream>
#include "Square.h"
#include "Ship.h"
#include <SDL_image.h>
#include <utility>
#include <set>
#include "Entity.h"
#include "GameParams.h"
#include <string>

/* This is an abstract base class for derived board classes */
class Board : public Entity {

public:
	static const int BOARD_WIDTH = 8;
	static const int BOARD_HEIGHT = 8; 
	static const int SQUARE_PIXEL_SIZE = 75;

	enum class Type {
		PLAYER,
		RADER
	};

	Board(Type type);

	/* Try to place a piece. Return whether or not it was a valid placement */
	bool Board::placeShip(Ship& ship, std::vector<std::pair<int, int>> coords);

	std::vector<Square> occupiedSquares();

	void print();

	/* Guess at where opponent's ship is placed. Return if hit or miss*/
	bool guess(const std::pair<int, int> coord, GameParams::Turn turn);

	/* A collection of ships active on the board. */
	std::vector<Ship> activeShips;

	/* A collection of squares which have been guessed for this board */
	std::vector<Square> guessedSquares;

	~Board();


protected:

	/* Deal damage to the ship at the current square */
	void damageHitShip(std::pair<int, int> coord, GameParams::Turn turn);

	/* Check if damage sunk ship and print to console */
	void damageSankShip(Ship ship, GameParams::Turn turn);

	bool validCoord(const std::pair<int, int> coord);

	/* The squares that make up the actual board. */
	std::vector<std::vector<Square>> squares;

	static const glm::vec2 PLAYER_BOARD_POSITION;
	static const glm::vec2 RADAR_BOARD_POSITION;
	static const glm::vec2 SIZE;
	
};
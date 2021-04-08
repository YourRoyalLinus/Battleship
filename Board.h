#pragma once
#include <vector>
#include <iostream>
#include "Square.h"
#include "Ship.h"
#include <SDL_image.h>
#include "Player.h"
#include <utility>
#include <set>
#include "Texture.h"

/* This is an abstract base class for derived board classes */
class Board {

public:
	static const int BOARD_WIDTH = 8;
	static const int BOARD_HEIGHT = 8; 
	static const int SQUARE_PIXEL_SIZE = 75;

	Board();

	/* Try to place a piece. Return whether or not it was a valid placement */
	bool Board::placeShip(Ship& ship, std::vector<std::pair<int, int>> coords);

	virtual void draw();

	std::vector<Square> occupiedSquares();
	void print();

	/* Guess at where opponent's ship is placed. Return if hit or miss*/
	bool guess(const std::pair<int, int> coord);

	/* A collection of ships active on the board. */
	std::vector<Ship> activeShips;

	virtual ~Board() = 0;


protected:

	/* Deal damage to the ship at the current square */
	void damageHitShip(std::pair<int, int> coord);

	bool validCoord(const std::pair<int, int> coord);

	/* The squares that make up the actual board. */
	std::vector<std::vector<Square>> squares;

	/* A collection of squares which have been guessed for this board */
	std::vector<Square> guessedSquares;

	/* The pretty grid picture */
	Texture gridTexture;

	/* Icon you display on a square if the player guessed that square and it was a miss */
	Texture missIconTexture;

	/* Icon you display on a square if the player guessed that square and it was a hit */
	Texture hitIconTexture;

	/* relative path of grid texture file */
	std::string gridImagePath;

	/* relative path of miss icon file */
	std::string missIconImagePath;

	/* relative path of hit icon file */
	std::string hitIconImagePath;

	
};

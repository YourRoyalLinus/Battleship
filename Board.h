#pragma once
#include <utility>
#include <set>
#include <SDL_image.h>
#include "GameParams.h"
#include "Entity.h"
#include "Square.h"
#include "Ship.h"

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

		/* Check to see if the player has already guessed this square */
		bool alreadyGuessedSquare(const std::pair<int, int> coord);

		/* Marked correct and incorrectly guessed squares when playing against another player */
		void markSquare(const std::pair<int, int> coord, bool prevRes);

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

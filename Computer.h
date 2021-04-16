#pragma once
#include "Board.h"
#include <set>

class Computer{
	public:
		enum class Difficulty {
			EASY,
			MEDIUM,
			HARD
		};

		virtual ~Computer() {
			delete board;
		}

		void GenerateBoard() {
			std::set<std::pair<int, int>> occupiedCoords;
			while (!ships.empty()) {
				Ship& computerShipRef = ships.back();
				int row = rand() % 8;
				int col = rand() % 8;
		
				while (occupiedCoords.find({ row, col }) != occupiedCoords.end()) { //Fixes rendering bug
					row = rand() % 8;
					col = rand() % 8;
					std::cout << row << " " << col << std::endl;
				}

				occupiedCoords.insert({ row, col });

				computerShipRef.snapToPosition({ row, col });

				if (board->placeShip(computerShipRef, computerShipRef.coords)) {
					ships.pop_back();
				}

				//state = GameState::PLAYING;
				board->print();
			}
		}

		virtual std::pair<int, int> GuessCoordinate() = 0;
		virtual void SankShip() = 0;

		std::pair<int, int> lastHitSqaure;
		bool hitStreak = false;
		//Game Variables
		Difficulty difficulty = Difficulty::MEDIUM;
		Board* board;
		std::vector<Ship> ships = { Ship(Ship::Type::CARRIER), Ship(Ship::Type::BATTLESHIP), Ship(Ship::Type::CRUISER), Ship(Ship::Type::SUBMARINE), Ship(Ship::Type::DESTROYER) };
	protected:
		//Advanced calculations variables
		int adjGuessIx = -1;
		std::pair<int, int> prevGuess;
		std::set<std::pair<int, int>> guesses;
		const std::vector<std::pair<int, int>> adjacentGuesses = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };
};
#pragma once
#include <set>
#include "Opponent.h"


class Computer : public Opponent{
	public:
		enum class Difficulty {
			EASY,
			MEDIUM,
			HARD
		};

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

		//Game Variables
		Difficulty difficulty = Difficulty::MEDIUM;
	protected:
		//Advanced calculations variables
		int adjGuessIx = -1;
		std::pair<int, int> prevGuess;
		std::set<std::pair<int, int>> guesses;
		const std::vector<std::pair<int, int>> adjacentGuesses = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };
};
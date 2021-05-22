#pragma once
#include "Opponent.h"

class Player : public Opponent{
	public:
		Player() {
			board = new Board(Board::Type::PLAYER);
		}
		Player(GameParams::Mode pvp) {
			board = new Board(Board::Type::RADER);
		}

	/* Not a huge fan of just hidding unimplemented overloads... */
	private:
			std::pair<int, int> GuessCoordinate() { 
				//NOT IMPLEMENTED FOR PLAYER 
				return { -1, -1};
			}

			void SankShip() { 
				//NOT IMPLEMENTED FOR PLAYER
				return;
			}
};


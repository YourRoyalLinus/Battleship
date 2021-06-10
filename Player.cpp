#include "Player.h"
#include "Game.h"
#include "InputHandler.h"
#include "Board.h"
#include "Ship.h"
#include "SinglePlayerGuess.h"
#include "EasyComputer.h"
#include "MediumComputer.h"
#include "HardComputer.h"
#include "NullStrategy.h"


Player::Player(Type type, GuessStrategy* guessStrategy) : guessStrategy(guessStrategy) {
	if (type == Type::HERO) {
		this->board = new Board(Board::Type::WATER);
	}
	else {
		this->board = new Board(Board::Type::RADER);
	}

	//TODO: There has got to be a better way of doing this than creating these lambdas here, but idk what I'm doing
	auto humanShipPlacement = [this]() {
		if (this->board->placeShip(*shipToPlace, shipToPlace->coords)) {
				this->ships.pop_back();
				if(!ships.empty())
					this->shipToPlace = &this->ships.back();
				this->board->print();
				std::cout << "\n\n" << std::endl;
		}
		};

	auto computerShipPlacement = [this]() {
		do {
			//If placing failed generate new coords & try again
			int row = rand() % 8;
			int col = rand() % 8;
			shipToPlace->snapToPosition({ row, col });
		} while (!this->board->placeShip(*shipToPlace, shipToPlace->coords));
		this->ships.pop_back();
		if(!ships.empty())
			this->shipToPlace = &this->ships.back();
		this->board->print();
	};

	switch (type) {
	case Type::HERO: {
		this->shipPlacementProc = humanShipPlacement;
	//	this->guessStrategy = new SinglePlayerGuess();
		break;
	}
	case Type::HUMAN_OPPONENT: {
		this->shipPlacementProc = []() {};
//		this->guessStrategy = new NullStrategy();
	}
	case Type::EASY_COMPUTER: {
		this->shipPlacementProc = computerShipPlacement;
	//	this->guessStrategy = new EasyComputer();
		break;
	}
	case Type::MEDIUM_COMPUTER: {
		this->shipPlacementProc = computerShipPlacement;
	//	this->guessStrategy = new MediumComputer();
		break;
	}
	case Type::HARD_COMPUTER: {
		this->shipPlacementProc = computerShipPlacement;
	//	this->guessStrategy = new HardComputer();
		break;
	}
	default:
		assert(false);
		guessStrategy = new MediumComputer();
		break;
	}

	shipToPlace = &ships.back();

}


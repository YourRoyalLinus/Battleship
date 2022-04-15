#pragma once
#include "Board.h"
#include "GuessStrategy.h"
#include <functional>
#include "ShipPlacementStrategy.h"
#include "PeerNetwork.h"

class Player {
public:

	static Player* createHeroSinglePlayer();
	static Player* createHeroMultiPlayer(PeerNetwork& net);
	static Player* createHumanOpponent();
	static Player* createEasyComputer();
	static Player* createMediumComputer();
	static Player* createHardComputer();
	static Player* createExpertComputer();

	void guess(Player& opponent) {	guessStrategy->guess(*this, opponent); }
	void placeShip() { placementStrategy->place(); }
	
	Board* board;
	std::vector<Ship> ships = { Ship(Ship::Type::CARRIER), Ship(Ship::Type::BATTLESHIP), Ship(Ship::Type::CRUISER), Ship(Ship::Type::SUBMARINE), Ship(Ship::Type::DESTROYER) };
	Ship* shipToPlace;

private:
	ShipPlacementStrategy* placementStrategy;
	GuessStrategy* guessStrategy;

	Player(Board* board, GuessStrategy* guessStrategy) : board(board), guessStrategy(guessStrategy) {}
	void setPlacementStrategy(ShipPlacementStrategy* placement) { this->placementStrategy = placement; }

};


#include "Player.h"
#include "Game.h"
#include "InputHandler.h"
#include "Board.h"
#include "Ship.h"
#include "SinglePlayerGuess.h"
#include "EasyComputer.h"
#include "MediumComputer.h"
#include "HardComputer.h"
#include "NullPlacement.h"
#include "NullGuess.h"
#include "HumanShipPlacement.h"
#include "ComputerShipPlacement.h"
#include "MultiPlayerGuess.h"



Player* Player::createHeroSinglePlayer() {
	Player* player = new Player(new Board(Board::Type::WATER), new SinglePlayerGuess());
	HumanShipPlacement* placementProc = new HumanShipPlacement(*player);
	player->setPlacementStrategy(placementProc);
	player->shipToPlace = &player->ships.back();
	return player;
}

Player* Player::createHeroMultiPlayer(PeerNetwork& net) {
	Player* player = new Player(new Board(Board::Type::WATER), new MultiPlayerGuess(net));
	HumanShipPlacement* placementProc = new HumanShipPlacement(*player);
	player->setPlacementStrategy(placementProc);
	player->shipToPlace = &player->ships.back();
	return player;
}

Player* Player::createHumanOpponent() {
	Player* player = new Player(new Board(Board::Type::RADER), new NullGuess());
	NullPlacement* placementProc = new NullPlacement(*player);
	player->setPlacementStrategy(placementProc);
	player->shipToPlace = &player->ships.back();
	return player;
}

Player* Player::createEasyComputer() {
	Player* player = new Player(new Board(Board::Type::RADER), new EasyComputer());
	ComputerShipPlacement* placementProc = new ComputerShipPlacement(*player);
	player->setPlacementStrategy(placementProc);
	player->shipToPlace = &player->ships.back();
	return player;
}

Player* Player::createMediumComputer() {
	Player* player = new Player(new Board(Board::Type::RADER), new MediumComputer());
	ComputerShipPlacement* placementProc = new ComputerShipPlacement(*player);
	player->setPlacementStrategy(placementProc);
	player->shipToPlace = &player->ships.back();
	return player;
}

Player* Player::createHardComputer() {
	Player* player = new Player(new Board(Board::Type::RADER), new HardComputer());
	ComputerShipPlacement* placementProc = new ComputerShipPlacement(*player);
	player->setPlacementStrategy(placementProc);
	player->shipToPlace = &player->ships.back();
	return player;
}
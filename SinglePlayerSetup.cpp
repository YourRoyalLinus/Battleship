#include "SinglePlayerSetup.h"
#include "InputHandler.h"
#include "PlaceShipCommand.h"
#include "RepositionShipCommand.h"
#include "RotateShipCommand.h"
#include "Player.h"
#include "Game.h"
#include "SinglePlayerPlayState.h"

void SinglePlayerSetup::update(Game& game) {
	Player* player = game.player;
	Player* opponent = game.opponent;

	InputHandler::setLeftClick(new PlaceShipCommand(*player));
	InputHandler::setMouseMove(new RepositionShipCommand(*player));
	InputHandler::setRightClick(new RotateShipCommand(*player));

	if (player->ships.empty()) {
		while (!opponent->ships.empty()) {
			opponent->placeShip();
		}
	}
	else {
		auto command = InputHandler::handleInput();
		if (command) {
			command->execute();
		}
	}

	if (player->ships.empty() && opponent->ships.empty()) {
		game.setup = false;
		game.state = new SinglePlayerPlayState();
	}


}
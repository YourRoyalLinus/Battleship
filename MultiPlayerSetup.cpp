#include "MultiPlayerSetup.h"
#include "InputHandler.h"
#include "PlaceShipCommand.h"
#include "RotateShipCommand.h"
#include "RepositionShipCommand.h"
#include "Game.h"
#include "MultiPlayerPlayState.h"

void MultiPlayerSetup::update(Game& game) {
	InputHandler::setLeftClick(new PlaceShipCommand(*game.player));
	InputHandler::setMouseMove(new RepositionShipCommand(*game.player));
	InputHandler::setRightClick(new RotateShipCommand(*game.player));

	auto command = InputHandler::handleInput();
	if (command) {
		command->execute();
	}

	if (game.player->ships.empty()) {
		//delete this
		game.setup = false;
		game.state = new MultiPlayerPlayState();
	}

}

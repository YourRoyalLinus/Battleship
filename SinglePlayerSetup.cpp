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
		game.activePlayer = player;
		game.inactivePlayer = opponent;
		game.state = new SinglePlayerPlayState();
	}


}

void SinglePlayerSetup::render(Game& game) {
	//update uniforms
	ResourceManager::getShader("grid").use().setFloat("iTime", game.mticks());
	ResourceManager::getShader("water").use().setFloat("iTime", game.mticks());
	ResourceManager::getShader("radar2").use().setFloat("time", game.mticks());

	game.opponent->board->draw(*game.radarBoardRenderer);
	game.player->board->draw(*game.waterRenderer);
	glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
	game.grid->draw(*game.gridRenderer);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//draw placed ships on player's board
	for (auto& ship : game.player->board->activeShips) {
		ship.draw(*game.shipRenderer);
	}

	if (!game.player->ships.empty()){
		Ship* shipToPlace = game.player->shipToPlace;
		//If there is a ship currently to place and it's current inside the player board part of the screen, draw it.
		if (shipToPlace->position.x >= 600.0f) {
			shipToPlace->draw(*game.shipRenderer);
		}
	
	}

	



}
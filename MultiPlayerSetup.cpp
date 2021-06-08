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
		game.state = new MultiPlayerPlayState();
	}

}

void MultiPlayerSetup::render(Game& game) {
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

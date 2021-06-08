#include "SinglePlayerPlayState.h"
#include "InputHandler.h"
#include "MakeGuessCommand.h"
#include "NullCommand.h"
#include "Marker.h"
#include "Game.h"

void SinglePlayerPlayState::update(Game& game) {
	Player* player = game.player;
	Player* opponent = game.opponent;
	Player* activePlayer = game.activePlayer;
	
	InputHandler::setLeftClick(new MakeGuessCommand(game));
	InputHandler::setMouseMove(new NullCommand());
	InputHandler::setRightClick(new NullCommand());

	if (game.activePlayer == player) {
		auto command = InputHandler::handleInput();
		if (command) {
			command->execute();
		}
	}
	else if (activePlayer == opponent) {
		opponent->guess(*player);
		game.endTurn();
	}

	if (player->board->activeShips.empty()) {
		std::cout << "Computer Won!" << std::endl;
		//TODO: CHANGE THIS!!
		exit(0);
	}
	else if (opponent->board->activeShips.empty()) {
		std::cout << "You've Won!" << std::endl;
		exit(0);
	}
}

void SinglePlayerPlayState::render(Game& game) {
	game.renderRadarPings();
	//update uniforms
	ResourceManager::getShader("grid").use().setFloat("iTime", game.mticks());
	ResourceManager::getShader("water").use().setFloat("iTime", game.mticks());
	//testing new radar effect.
	//TODO: these don't need to be done with uniforms right now.
	ResourceManager::getShader("radar2").use().setFloat("time", game.mticks());
	//Render to off-screen buffer for postprocessing effects
	game.effects->beginRender();
	game.opponent->board->draw(*game.radarBoardRenderer);
	game.player->board->draw(*game.waterRenderer);
	glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
	game.grid->draw(*game.gridRenderer);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	//Draw miss markers where opponent guessed wrong
	for(auto square : game.player->board->guessedSquares){
		if (!square.occupied) {
			Marker miss(Marker::Type::MISS, glm::vec2(600 + square.col * GameParams::SQUARE_PIXEL_SIZE, square.row * GameParams::SQUARE_PIXEL_SIZE));
			miss.draw(*game.spriteRenderer);
		}
	}
	//draw placed ships on player's board
	for (auto& ship : game.player->board->activeShips) {
		ship.draw(*game.shipRenderer);
	}
	
	for (auto& fireEmitter : game.fireEmitters) {
		fireEmitter.draw();
	}
	for (auto& smokeEmitter : game.smokeEmitters) {
		smokeEmitter.draw();
	}
	game.removeUnderwaterFire();
	//after redering whole scene to off-screen buffer, apply postprocessing affects and blit to screen.
	game.effects->endRender();
	game.effects->render(game.shakeTime);

}

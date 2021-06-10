#include "MultiPlayerPlayState.h"
#include "Game.h"
#include "InputHandler.h"
#include "Marker.h"

void MultiPlayerPlayState::update() {
	Player* player = game.player;
	Player* opponent = game.opponent;
	Player* activePlayer = game.activePlayer;

	if (game.activePlayer == player) {
		Event* event = InputHandler::handleInput();
		if (event == nullptr) { return; }
		else if (event->eventType == Event::Type::LEFT_CLICK) {
			player->guess(*opponent);
			game.endTurn();
		}
	}
	else if (game.activePlayer == opponent) {
		auto buffer = game.net->ReceiveData();
		if (buffer.bufferType == Payload::BufferType::GUESS_RESULT) {
			opponent->board->markSquare({ buffer.xChoordGuess, buffer.yChoordGuess }, buffer.prevGuessHit);
		}
		else if (buffer.bufferType == Payload::BufferType::NEXT_GUESS) {
			std::pair<int, int> oppGuess = { buffer.xChoordGuess, buffer.yChoordGuess };
			bool opponentHit = player->board->guess(oppGuess);

			if (opponentHit) {
				game.spawnFire(oppGuess);
				game.shakeScreen();
			}

			int resp = 0;
			if (player->board->activeShips.empty()) {
				resp = game.net->SendGameOver();
				//TODO: add game over state
				exit(-1);
				return;
			}
			else {
				resp = game.net->SendGuessResult(oppGuess.first, oppGuess.second, opponentHit);
				game.endTurn();
			}

			//Failed to get a response from the other player before the timeout period expired (60 seconds)
			if (resp == 0) {
				std::cout << "Lost connection to peer...\n";
				std::cout << "Game Over!\n";
				exit(-1);
				return;
			}
			//game.endTurn();
		}
		else if (buffer.bufferType == Payload::BufferType::GAME_OVER) {
			std::cout << "You've Won!\n";
			exit(-1);
			return;
		}
	}

}

void MultiPlayerPlayState::render() {
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

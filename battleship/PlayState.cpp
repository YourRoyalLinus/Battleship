#include "PlayState.h"
#include "Game.h"
#include "Marker.h"

PlayState::PlayState(Game& game) : GameState(game)
{
	game.player->board->addObserver(&game);
}

void PlayState::render() {
	game.renderRadarPings();
	//update uniforms
	if (game.activePlayer == game.player) {
		ResourceManager::getShader("radar2").use().setFloat("turn", 1.0f);
	}
	else {
		ResourceManager::getShader("radar2").use().setFloat("turn", 0.0f);
	}

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

	//if (game.activePlayer == game.player) {
	//	turnPrompt.draw(*game.spriteRenderer);
	//}
	
	//Draw miss markers where opponent guessed wrong
	for(auto square : game.player->board->guessedSquares){
		if (!square.occupied) {
			Marker miss(Marker::Type::MISS, glm::vec2(600 + square.col * Game::SQUARE_PIXEL_SIZE, square.row * Game::SQUARE_PIXEL_SIZE));
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

#include "SetupState.h"
#include "Game.h"

SetupState::SetupState(Game& game) : GameState(game)
{
	text.color = glm::vec4(0.2f, 1.0f, 0.6f, 1.0f);
	text.setFlashing(true);
	text.setBouncing(true);
	text.scale(.7);
}

void SetupState::render() 
{
	//update uniforms
	ResourceManager::getShader("grid").use().setFloat("iTime", game.mticks());
	ResourceManager::getShader("water").use().setFloat("iTime", game.mticks());
	ResourceManager::getShader("radar2").use().setFloat("time", game.mticks());

	
	game.opponent->board->draw(*game.radarBoardRenderer);
	game.player->board->draw(*game.waterRenderer);
	glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
	game.grid->draw(*game.gridRenderer);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	text.draw(*game.spriteRenderer);

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

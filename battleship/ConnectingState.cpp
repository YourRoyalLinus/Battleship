#include "ConnectingState.h"
#include "Game.h"
#include "MultiPlayerSetup.h"

ConnectingState::ConnectingState(Game& game) : GameState(game) {}


void ConnectingState::update() {
	if (searching) {
		text = new UIComponent(UIComponent::Alignment::CENTER, ResourceManager::getTexture("searching"), 1.0f);
		if (game.net->searchForOpponent(game.networkStartIp)) {
			//someone was already hosting a game
			game.activePlayer = game.opponent;
			game.inactivePlayer = game.player;
			game.state = new MultiPlayerSetup(game);
		}
		else {
			searching = false;
			game.net->hostGame();
			text = new UIComponent(UIComponent::Alignment::CENTER, ResourceManager::getTexture("waiting"), 1.0f);
			text->setBouncing(true);
		}
	}
	else {
		if (game.net->findOpponent()) {
			//You we're hosting a game and found an opponent
			game.activePlayer = game.player;
			game.inactivePlayer = game.opponent;
			game.state = new MultiPlayerSetup(game);
		}
		text->update(game.mticks());
	}

}

void ConnectingState::render() {
	ResourceManager::getShader("water").use().setFloat("iTime", game.mticks());
	ResourceManager::getShader("radar2").use().setFloat("time", game.mticks());
	game.menuWater->draw(*game.waterRenderer);
	if (text != nullptr) {
		text->draw(*game.spriteRenderer);
	}

}

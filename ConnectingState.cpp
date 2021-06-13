#include "ConnectingState.h"
#include "Game.h"
#include "MultiPlayerSetup.h"

ConnectingState::ConnectingState(Game& game) : GameState(game) {}


void ConnectingState::update() {
	if (searching) {
		if (game.net->searchForOpponent(game.networkStartIp)) {
			//someone was already hosting a game
			game.activePlayer = game.opponent;
			game.inactivePlayer = game.player;
			game.state = new MultiPlayerSetup(game);
		}
		else {
			searching = false;
			game.net->hostGame();
		}
	}
	else {
		if (game.net->findOpponent()) {
			//You we're hosting a game and found an opponent
			game.activePlayer = game.player;
			game.inactivePlayer = game.opponent;
			game.state = new MultiPlayerSetup(game);
		}
	}

}

void ConnectingState::render() {
	std::cout << "Waiting for another player..." << std::endl;
}

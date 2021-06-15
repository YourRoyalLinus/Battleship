#include "SinglePlayerPlayState.h"
#include "InputHandler.h"
#include "Marker.h"
#include "Game.h"

SinglePlayerPlayState::SinglePlayerPlayState(Game& game) : PlayState(game) 
{
}

void SinglePlayerPlayState::update() {
	Player& player = *game.player;
	Player& opponent = *game.opponent;
	Player& activePlayer = *game.activePlayer;
	
	if (game.activePlayer == game.player) {
	//	turnPrompt.update(game.mticks());

		Event* event = InputHandler::handleInput();
		if (event == nullptr) { return; }
		else if (event->eventType == Event::Type::LEFT_CLICK) {
			player.guess(opponent);
			game.endTurn();
		}
		delete event;
	}
	else if (game.activePlayer == game.opponent) {
		opponent.guess(player);
		game.endTurn();
	}

	if (player.board->activeShips.empty()) {
		std::cout << "Computer Won!" << std::endl;
		//TODO: CHANGE THIS!!
		exit(0);
	}
	else if (opponent.board->activeShips.empty()) {
		std::cout << "You've Won!" << std::endl;
		exit(0);
	}

}


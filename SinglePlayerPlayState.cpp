#include "SinglePlayerPlayState.h"
#include "Game.h"
#include "InputHandler.h"
#include "MakeGuessCommand.h"
#include "NullCommand.h"

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

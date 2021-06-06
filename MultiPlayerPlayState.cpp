#include "MultiPlayerPlayState.h"
#include "Game.h"
#include "InputHandler.h"
#include "MakeGuessCommand.h"
#include "NullCommand.h"

void MultiPlayerPlayState::update(Game& game) {
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
			//auto guess = player->guess;
			//if (guess) {

			//	int resp = game.net->SendNextGuess(guess->first, guess->second);
			//	//Unable to send data to the other player before timeout period expired(60 seconds)
			//	if (resp == 0) {
			//		std::cout << "Lost connection to peer...\n";
			//		std::cout << "Game Over!\n";
			//		//TODO: add game over state
			//		exit(-1);
			//		return;
			//	}

			//	player->guess = nullptr;
			//	game.endTurn();
			//}
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
			}

			//Failed to get a response from the other player before the timeout period expired (60 seconds)
			if (resp == 0) {
				std::cout << "Lost connection to peer...\n";
				std::cout << "Game Over!\n";
				exit(-1);
				return;
			}
			game.endTurn();
		}
		else if (buffer.bufferType == Payload::BufferType::GAME_OVER) {
			std::cout << "You've Won!\n";
			exit(-1);
			return;
		}
	}



}

#include "MultiPlayerPlayState.h"
#include "Game.h"
#include "InputHandler.h"
#include "Marker.h"

MultiPlayerPlayState::MultiPlayerPlayState(Game& game) : PlayState(game) 
{ 

}


void MultiPlayerPlayState::update() {
	Player* player = game.player;
	Player* opponent = game.opponent;
	Player* activePlayer = game.activePlayer;

	if (game.activePlayer == player) {
		//Your turn
		auto buffer = game.net->ReceiveData();
		if (buffer.bufferType == Payload::BufferType::GUESS_RESULT) {
			opponent->board->markSquare({ buffer.xChoordGuess, buffer.yChoordGuess }, buffer.prevGuessHit);
			game.endTurn();
		}


		Event* event = InputHandler::handleInput();
		if (event == nullptr) { return; }
		else if (event->eventType == Event::Type::LEFT_CLICK) {
			if (!guessedThisTurn) {
				player->guess(*opponent);
				guessedThisTurn = true;
			}
		}
	}


	else if (game.activePlayer == opponent) {
		guessedThisTurn = false;
		//Your opponent's turn
		auto buffer = game.net->ReceiveData();

		if (buffer.bufferType == Payload::BufferType::EMPTY) { return; }
		else if (buffer.bufferType == Payload::BufferType::GUESS_RESULT) {
			opponent->board->markSquare({ buffer.xChoordGuess, buffer.yChoordGuess }, buffer.prevGuessHit);
		}
		else if (buffer.bufferType == Payload::BufferType::NEXT_GUESS) {
			std::pair<int, int> oppGuess = { buffer.xChoordGuess, buffer.yChoordGuess };
			bool opponentHit = player->board->guess(oppGuess);

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
		}
		else if (buffer.bufferType == Payload::BufferType::GAME_OVER) {
			std::cout << "You've Won!\n";
			exit(-1);
			return;
		}
	}

}

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
		Event* event = InputHandler::handleInput();
		if (event == nullptr) { return; }
		else if (event->eventType == Event::Type::LEFT_CLICK) {
			int row = (InputHandler::mouseY) / Board::SQUARE_PIXEL_SIZE; 
			int col = (InputHandler::mouseX) / Board::SQUARE_PIXEL_SIZE;

			if (opponent.board->alreadyGuessedSquare({ row, col })) {
				std::cout << "ROW: " << row << " COLUMN: " << col << " has already been guessed. Please Guess again." << std::endl;
				return;
			}

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
		exit(0);
	}
	else if (opponent.board->activeShips.empty()) {
		std::cout << "You've Won!" << std::endl;
		exit(0);
	}

}


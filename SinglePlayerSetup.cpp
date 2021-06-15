#include "SinglePlayerSetup.h"
#include "InputHandler.h"
#include "Player.h"
#include "Game.h"
#include "SinglePlayerPlayState.h"
#include "Board.h"

SinglePlayerSetup::SinglePlayerSetup(Game& game) : SetupState(game)
{

}

void SinglePlayerSetup::update() {
	text.update(game.mticks());

	Player* player = game.player;
	Player* opponent = game.opponent;

	if (player->ships.empty()) {
		while (!opponent->ships.empty()) {
			opponent->placeShip();
		}
	}
	else {
		Event* event = InputHandler::handleInput();
		if (event == nullptr) { return; }
		switch (event->eventType) {
		case Event::Type::MOUSE_MOVE: {
			//reposition active ship
			MouseMove* mouseMove = static_cast<MouseMove*>(event);
			int row = mouseMove->mouseY / Board::SQUARE_PIXEL_SIZE;
			int col = (mouseMove->mouseX - 600.0f) / Board::SQUARE_PIXEL_SIZE;
			player->shipToPlace->snapToPosition({ row, col });
			break;
		}
		case Event::Type::LEFT_CLICK: {
			player->placeShip();
			break;
		}
		case Event::Type::RIGHT_CLICK: {
			//rotate active ship
			player->shipToPlace->rotate();
			break;
		}
		}
		delete event;
	}

	if (player->ships.empty() && opponent->ships.empty()) {
		game.activePlayer = player;
		game.inactivePlayer = opponent;
		game.state = new SinglePlayerPlayState(game);
	}

}



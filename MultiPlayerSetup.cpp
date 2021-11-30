#include "MultiPlayerSetup.h"
#include "InputHandler.h"
#include "Game.h"
#include "MultiPlayerPlayState.h"
#include "Player.h"
#include "Peer.h"

MultiPlayerSetup::MultiPlayerSetup(Game& game) : SetupState(game) 
{
}

void MultiPlayerSetup::update() {
	text.update(game.mticks());

	Player* player = game.player;
	if (player->ships.empty()) {
		game.state = new MultiPlayerPlayState(game);
		return;
	}

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

}


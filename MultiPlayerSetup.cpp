#include "MultiPlayerSetup.h"
#include "InputHandler.h"
#include "Game.h"
#include "MultiPlayerPlayState.h"
#include "Player.h"
#include "Peer.h"

MultiPlayerSetup::MultiPlayerSetup(Game& game) : GameState(game) {
	game.net->ConnectToRandomOpponent(game.networkStartIp);
	if (game.net->peerType == Peer::PeerType::HOSTING_PEER) {
		game.activePlayer = game.player;
		game.inactivePlayer = game.opponent;
	}
	else if (game.net->peerType == Peer::PeerType::CONNECTING_PEER) {
		game.activePlayer = game.opponent;
		game.inactivePlayer = game.player;
	}

}

void MultiPlayerSetup::update() {
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
		//place active ship
		LeftClick* leftClick = static_cast<LeftClick*>(event);
		int row = leftClick->mouseY / Board::SQUARE_PIXEL_SIZE;
		int col = (leftClick->mouseX - 600.0f) / Board::SQUARE_PIXEL_SIZE;
		player->board->placeShip(*player->shipToPlace, player->shipToPlace->coords);
		player->ships.pop_back();
		if (!player->ships.empty())
			player->shipToPlace = &player->ships.back();
		break;
	}
	case Event::Type::RIGHT_CLICK: {
		//rotate active ship
		player->shipToPlace->rotate();
		break;
	}
	}




}

void MultiPlayerSetup::render() {
	//update uniforms
	ResourceManager::getShader("grid").use().setFloat("iTime", game.mticks());
	ResourceManager::getShader("water").use().setFloat("iTime", game.mticks());
	ResourceManager::getShader("radar2").use().setFloat("time", game.mticks());

	game.opponent->board->draw(*game.radarBoardRenderer);
	game.player->board->draw(*game.waterRenderer);
	glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
	game.grid->draw(*game.gridRenderer);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//draw placed ships on player's board
	for (auto& ship : game.player->board->activeShips) {
		ship.draw(*game.shipRenderer);
	}

	if (!game.player->ships.empty()) {
		Ship* shipToPlace = game.player->shipToPlace;
		//If there is a ship currently to place and it's current inside the player board part of the screen, draw it.
		if (shipToPlace->position.x >= 600.0f) {
			shipToPlace->draw(*game.shipRenderer);
		}

	}

}

#include "MenuState.h"
#include "Game.h"
#include "SinglePlayerSetup.h"
#include <iostream>

void MenuState::update(Game& game) {
	std::string input;
	std::cout << "Select a game mode:\n1. Player vs Player\n2. Computer\n";
	std::cin >> input;

	switch (stoi(input)) {
		case 1://PLAYER		
			game.mode = GameParams::Mode::PVP;
			game.net->ConnectToRandomOpponent(game.networkStartIp);
			game.opponent = new Player(Player::Type::HUMAN);
			if (game.net->peerType == Peer::PeerType::HOSTING_PEER) {
				game.activePlayer = game.player;
				game.inactivePlayer = game.opponent;
			}
			else if (game.net->peerType == Peer::PeerType::CONNECTING_PEER) {
				game.activePlayer = game.opponent;
				game.inactivePlayer = game.player;
			}
			break;
		case 2: //COMPUTER
			game.mode = (GameParams::Mode::SOLO);
			std::cout << "\nSelect Computer Difficulty:\n1. EASY\n2. MEDIUM (Default)\n3. HARD\n";
			std::cin >> input;
			switch (stoi(input)) {
				case 1:
					game.opponent = new Player(Player::Type::EASY_COMPUTER);
					break;
				case 2:
					game.opponent = new Player(Player::Type::MEDIUM_COMPUTER);
					break;
				case 3:
					game.opponent = new Player(Player::Type::HARD_COMPUTER);
					break;
				default:
					game.opponent = new Player(Player::Type::EASY_COMPUTER);
					break;
			}
			game.activePlayer = game.player;
			game.inactivePlayer = game.opponent;
			game.state = new SinglePlayerSetup();
			break;
	}
	//HACK DELETE THIS
	game.setup = true;

}

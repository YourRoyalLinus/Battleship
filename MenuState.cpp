#include "MenuState.h"
#include "Game.h"
#include "SinglePlayerSetup.h"
#include <iostream>
#include "ResourceManager.h"
#include "Menu.h"
#include "MenuItem.h"
#include "InputHandler.h"
#include "MenuCommand.h"
#include "MenuSelectEvent.h"


void MenuState::update(Game& game) {
	if (game.menus.empty()) {	
		Menu* startMenu = new Menu();
		startMenu->addMenuItem(new MenuItem(MenuItem::ItemType::PVP, glm::vec2(600.0f - (510.0f/2.0f), 50.0f), glm::vec2(510.0f, 110.0f), ResourceManager::getTexture("title")));
		startMenu->addMenuItem(new MenuItem(MenuItem::ItemType::SOLO, glm::vec2(600.0f - (200.0f/2.0f), 220.0f), glm::vec2(200.0f, 90.0f), ResourceManager::getTexture("solo")));
		startMenu->addMenuItem(new MenuItem(MenuItem::ItemType::PVP, glm::vec2(600.0f - (200.0f/2.0f), 370.0f), glm::vec2(200.0f, 90.0f), ResourceManager::getTexture("pvp")));
		startMenu->init();
		game.menus.push(startMenu);
	}
	else {
		Menu& activeMenu = *game.menus.top();
		//This is retarded.
		InputHandler::setKeyDown(new MenuMoveDown(activeMenu));
		InputHandler::setKeyUp(new MenuMoveUp(activeMenu));
		InputHandler::setEnterKey(new MenuSelect(game, activeMenu));

		auto command = InputHandler::handleInput();
		if (command) {
			command->execute();
		}
	}

	//std::string input;
	//std::cout << "Select a game mode:\n1. Player vs Player\n2. Computer\n";
	//std::cin >> input;

	//switch (stoi(input)) {
	//	case 1://PLAYER		
	//		game.mode = GameParams::Mode::PVP;
	//		game.net->ConnectToRandomOpponent(game.networkStartIp);
	//		game.opponent = new Player(Player::Type::HUMAN);
	//		if (game.net->peerType == Peer::PeerType::HOSTING_PEER) {
	//			game.activePlayer = game.player;
	//			game.inactivePlayer = game.opponent;
	//		}
	//		else if (game.net->peerType == Peer::PeerType::CONNECTING_PEER) {
	//			game.activePlayer = game.opponent;
	//			game.inactivePlayer = game.player;
	//		}
	//		break;
	//	case 2: //COMPUTER
	//		game.mode = (GameParams::Mode::SOLO);
	//		std::cout << "\nSelect Computer Difficulty:\n1. EASY\n2. MEDIUM (Default)\n3. HARD\n";
	//		std::cin >> input;
	//		switch (stoi(input)) {
	//			case 1:
	//				game.opponent = new Player(Player::Type::EASY_COMPUTER);
	//				break;
	//			case 2:
	//				game.opponent = new Player(Player::Type::MEDIUM_COMPUTER);
	//				break;
	//			case 3:
	//				game.opponent = new Player(Player::Type::HARD_COMPUTER);
	//				break;
	//			default:
	//				game.opponent = new Player(Player::Type::EASY_COMPUTER);
	//				break;
	//		}
	//		game.activePlayer = game.player;
	//		game.inactivePlayer = game.opponent;
	//		game.state = new SinglePlayerSetup();
	//		break;
	//}
	////HACK DELETE THIS
	//game.setup = true;
}



void MenuState::render(Game& game) {
	ResourceManager::getShader("water").use().setFloat("iTime", game.mticks());
	ResourceManager::getShader("radar2").use().setFloat("time", game.mticks());

	game.menuWater->draw(*game.waterRenderer);
	game.menus.top()->draw();


}

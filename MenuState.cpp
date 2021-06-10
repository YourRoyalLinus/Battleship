#include "MenuState.h"
#include "Game.h"
#include "SinglePlayerSetup.h"
#include "MultiPlayerSetup.h"
#include <iostream>
#include "ResourceManager.h"
#include "Menu.h"
#include "MenuItem.h"
#include "InputHandler.h"
#include "Event.h"
#include "NullStrategy.h"
#include "MultiPlayerGuess.h"
#include "SinglePlayerGuess.h"
#include "EasyComputer.h"
#include "MediumComputer.h"
#include "HardComputer.h"

MenuState::MenuState(Game& game) : GameState(game) {
	Menu* startMenu = new Menu();
	startMenu->addMenuItem(new MenuItem(MenuItem::ItemType::PVP, glm::vec2(600.0f - (510.0f / 2.0f), 50.0f), glm::vec2(510.0f, 110.0f), ResourceManager::getTexture("title")));
	startMenu->addMenuItem(new MenuItem(MenuItem::ItemType::SOLO, glm::vec2(600.0f - (200.0f / 2.0f), 220.0f), glm::vec2(200.0f, 90.0f), ResourceManager::getTexture("solo")));
	startMenu->addMenuItem(new MenuItem(MenuItem::ItemType::PVP, glm::vec2(600.0f - (200.0f / 2.0f), 370.0f), glm::vec2(200.0f, 90.0f), ResourceManager::getTexture("pvp")));
	startMenu->init();
	game.menus.push(startMenu);
}


void MenuState::update() {
	Menu& activeMenu = *game.menus.top();
	auto event = InputHandler::handleInput();
	if (event == nullptr) { return; }
	switch (event->eventType) {
	case Event::Type::ARROW_DOWN:
		activeMenu.moveSelectedDown();
		break;
	case Event::Type::ARROW_UP:
		activeMenu.moveSelectedUp();
		break;
	case Event::Type::ENTER:
		selectMenuOption(*activeMenu.selectedItem);
		break;
	}
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
	//	}
}

void MenuState::render() {
	ResourceManager::getShader("water").use().setFloat("iTime", game.mticks());
	ResourceManager::getShader("radar2").use().setFloat("time", game.mticks());

	game.menuWater->draw(*game.waterRenderer);
	game.menus.top()->draw();


}

void MenuState::selectMenuOption(MenuItem& menuItem) {
	switch (menuItem.type) {
	case MenuItem::ItemType::SOLO: {
		//create difficulty menu
		Menu* difficultyMenu = new Menu();
		difficultyMenu->addMenuItem(new MenuItem(MenuItem::ItemType::EASY, glm::vec2(600.0f - (200.0f / 2.0f), 80.0f), glm::vec2(200.0f, 90.0f), ResourceManager::getTexture("easy")));
		difficultyMenu->addMenuItem(new MenuItem(MenuItem::ItemType::MEDIUM, glm::vec2(600.0f - (200.0f / 2.0f), 180.0f), glm::vec2(200.0f, 90.0f), ResourceManager::getTexture("medium")));
		difficultyMenu->addMenuItem(new MenuItem(MenuItem::ItemType::HARD, glm::vec2(600.0f - (200.0f / 2.0f), 280.0f), glm::vec2(200.0f, 90.0f), ResourceManager::getTexture("hard")));
		difficultyMenu->addMenuItem(new MenuItem(MenuItem::ItemType::BACK, glm::vec2(600.0f - (200.0f / 2.0f), 380.0f), glm::vec2(200.0f, 90.0f), ResourceManager::getTexture("back")));
		difficultyMenu->init();
		game.menus.push(difficultyMenu);
		break;
	}
	case MenuItem::ItemType::PVP:
		game.net = new PeerNetwork();
		game.player = new Player(Player::Type::HERO, new MultiPlayerGuess(game.net));
		game.opponent = new Player(Player::Type::HUMAN_OPPONENT, new NullStrategy());
		game.state = new MultiPlayerSetup(game);
		break;
	case MenuItem::ItemType::EASY:
		game.player = new Player(Player::Type::HERO, new SinglePlayerGuess());
		game.opponent = new Player(Player::Type::EASY_COMPUTER, new EasyComputer());
		game.state = new SinglePlayerSetup(game);
		break;
	case MenuItem::ItemType::MEDIUM:
		game.player = new Player(Player::Type::HERO, new SinglePlayerGuess());
		game.opponent = new Player(Player::Type::MEDIUM_COMPUTER, new MediumComputer());
		game.state = new SinglePlayerSetup(game);
		break;
	case MenuItem::ItemType::HARD:
		game.player = new Player(Player::Type::HERO, new SinglePlayerGuess());
		game.opponent = new Player(Player::Type::HARD_COMPUTER, new HardComputer());
		game.state = new SinglePlayerSetup(game);
		break;
	case MenuItem::ItemType::BACK:
		game.menus.pop();
		break;
	default:
		break;
	}

}

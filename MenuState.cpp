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
#include "NullGuess.h"
#include "MultiPlayerGuess.h"
#include "SinglePlayerGuess.h"
#include "EasyComputer.h"
#include "MediumComputer.h"
#include "HardComputer.h"
#include "ConnectingState.h"

MenuState::MenuState(Game& game) : GameState(game) {
	Menu* startMenu = new Menu();
	UIComponent* title = new UIComponent(UIComponent::Alignment::TOP, ResourceManager::getTexture("title"), 1.0f);
	title->scale(1.5f);
	startMenu->setTitle(title);
	startMenu->addMenuItem(new MenuItem(MenuItem::ItemType::SOLO, glm::vec2(600.0f - (200.0f / 2.0f), 220.0f), glm::vec2(200.0f, 90.0f), ResourceManager::getTexture("solo")));
	startMenu->addMenuItem(new MenuItem(MenuItem::ItemType::PVP, glm::vec2(600.0f - (200.0f / 2.0f), 370.0f), glm::vec2(200.0f, 90.0f), ResourceManager::getTexture("pvp")));
	startMenu->init();
	game.menus.push(startMenu);
}


void MenuState::update() {
	Menu& activeMenu = *game.menus.top();
	activeMenu.update(game.mticks());
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
		difficultyMenu->addMenuItem(new MenuItem(MenuItem::ItemType::EXPERT, glm::vec2(600.0f - (200.0f / 2.0f), 380.0f), glm::vec2(200.0f, 90.0f), ResourceManager::getTexture("hard"))); //TODO: CREATE EXPERT TEXTURE
		difficultyMenu->addMenuItem(new MenuItem(MenuItem::ItemType::BACK, glm::vec2(600.0f - (200.0f / 2.0f), 480.0f), glm::vec2(200.0f, 90.0f), ResourceManager::getTexture("back")));
		difficultyMenu->init();
		game.menus.push(difficultyMenu);
		break;
	}
	case MenuItem::ItemType::PVP:
		game.net = new PeerNetwork();
		game.player = Player::createHeroMultiPlayer(*game.net);
		game.opponent = Player::createHumanOpponent();
		game.state = new ConnectingState(game);
		break;
	case MenuItem::ItemType::EASY:
		game.player = Player::createHeroSinglePlayer();
		game.opponent = Player::createEasyComputer();
		game.state = new SinglePlayerSetup(game);
		break;
	case MenuItem::ItemType::MEDIUM:
		game.player = Player::createHeroSinglePlayer();
		game.opponent = Player::createMediumComputer();
		game.state = new SinglePlayerSetup(game);
		break;
	case MenuItem::ItemType::HARD:
		game.player = Player::createHeroSinglePlayer();
		game.opponent = Player::createHardComputer();
		game.state = new SinglePlayerSetup(game);
		break;
	case MenuItem::ItemType::EXPERT:
		game.player = Player::createHeroSinglePlayer();
		game.opponent = Player::createExpertComputer();
		game.state = new SinglePlayerSetup(game);
		break;
	case MenuItem::ItemType::BACK:
		game.menus.pop();
		break;
	default:
		break;
	}

}

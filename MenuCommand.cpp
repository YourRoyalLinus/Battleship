#include "MenuCommand.h"
#include "Game.h"
#include "SinglePlayerSetup.h"
#include "MultiPlayerSetup.h"
void MenuMoveUp::execute() {
	menu.moveSelectedUp();
}

void MenuMoveDown::execute() {
	menu.moveSelectedDown();
}

void MenuSelect::execute() {
	switch (menu.selectedItem->type) {
	case MenuItem::ItemType::SOLO: {
		Menu* difficultyMenu = new Menu();
		difficultyMenu->addMenuItem(new MenuItem(MenuItem::ItemType::EASY, glm::vec2(600.0f - (200.0f / 2.0f), 50.0f), glm::vec2(200.0f, 90.0f), ResourceManager::getTexture("easy")));
		difficultyMenu->addMenuItem(new MenuItem(MenuItem::ItemType::MEDIUM, glm::vec2(600.0f - (200.0f / 2.0f), 200.0f), glm::vec2(200.0f, 90.0f), ResourceManager::getTexture("medium")));
		difficultyMenu->addMenuItem(new MenuItem(MenuItem::ItemType::HARD, glm::vec2(600.0f - (200.0f / 2.0f), 350.0f), glm::vec2(200.0f, 90.0f), ResourceManager::getTexture("hard")));
		difficultyMenu->addMenuItem(new MenuItem(MenuItem::ItemType::BACK, glm::vec2(600.0f - (200.0f / 2.0f), 500.0f), glm::vec2(200.0f, 90.0f), ResourceManager::getTexture("back")));
		difficultyMenu->init();
		game.menus.push(difficultyMenu);
		break;
	}
	case MenuItem::ItemType::PVP: 
		game.opponent = new Player(Player::Type::HUMAN_OPPONENT);
		game.state = new MultiPlayerSetup();
		break;
	case MenuItem::ItemType::BACK:
		game.menus.pop();
		break;
	case MenuItem::ItemType::EASY: 
		game.opponent = new Player(Player::Type::EASY_COMPUTER);
		game.state = new SinglePlayerSetup();
		break;
	case MenuItem::ItemType::MEDIUM: 
		game.opponent = new Player(Player::Type::MEDIUM_COMPUTER);
		game.state = new SinglePlayerSetup();
		break;
	case MenuItem::ItemType::HARD: 
		game.opponent = new Player(Player::Type::HARD_COMPUTER);
		game.state = new SinglePlayerSetup();
		break;
	
	}
}
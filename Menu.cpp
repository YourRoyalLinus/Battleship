#include "Menu.h"

Menu::~Menu() {
	delete this->menuItemRenderer;
	delete this->cursor;
	delete this->title;
	menuItems.clear();
}

void Menu::init() {
	if (!menuItems.empty()) {
		selectedItem = menuItems.front();
		cursor->scale(0.3f); //BADD
		cursor->color = glm::vec4(1.0f, 0.1f, 0.34f, 1.0f);
		moveCursorToSelectedItem();
		cursor->setBouncing(true);
	}
}

void Menu::addMenuItem(MenuItem* item) {
	menuItems.push_back(item);
}

void Menu::setTitle(UIComponent* title) {
	this->title = title;
}

void Menu::moveSelectedDown() {
	auto iterPos = std::find(menuItems.begin(), menuItems.end(), selectedItem);
	if (++iterPos != menuItems.end()) {
		selectedItem = *iterPos;
		moveCursorToSelectedItem();
	}
}

void Menu::moveSelectedUp() {
	auto iterPos = std::find(menuItems.begin(), menuItems.end(), selectedItem);
	if (iterPos != menuItems.begin()) {
		selectedItem = *(--iterPos);
		moveCursorToSelectedItem();
	}
}

void Menu::update(float dt) {
	cursor->update(dt);
}

void Menu::draw() {
	if (title != nullptr) {
		title->draw(*menuItemRenderer);
	}
	for (MenuItem* item : menuItems) {
		//reset color of non-selected item's. better way to do this for sure.
		item->color = glm::vec4(1.0f);
	}
	selectedItem->color = glm::vec4(1.0f, 0.1f, 0.3f, 1.0f);
	for (MenuItem* item : menuItems) {
			item->draw(*menuItemRenderer);
	}
	cursor->draw(*menuItemRenderer);
}

void Menu::moveCursorToSelectedItem() {
	cursor->position = selectedItem->position;
	cursor->position.x += selectedItem->size.x + 5.0f;
	cursor->position.y += (selectedItem->size.y - cursor->size.y) / 2;
}

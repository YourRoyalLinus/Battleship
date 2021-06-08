#include "Menu.h"
#include "MenuSelectEvent.h"

void Menu::init() {
	if (!menuItems.empty()) {
		selectedItem = menuItems.front();
	}
}

void Menu::moveSelectedDown() {
	auto iterPos = std::find(menuItems.begin(), menuItems.end(), selectedItem);
	if (++iterPos != menuItems.end()) {
		selectedItem = *iterPos;
	}
}

void Menu::moveSelectedUp() {
	auto iterPos = std::find(menuItems.begin(), menuItems.end(), selectedItem);
	if (iterPos != menuItems.begin()) {
		selectedItem = *(--iterPos);
	}
}

void Menu::draw() {
	for (MenuItem* item : menuItems) {
		//reset color of non-selected item's. better way to do this for sure.
		item->color = glm::vec4(1.0f);
	}
	selectedItem->color = glm::vec4(1.0f, 0.1f, 0.3f, 1.0f);
	for (MenuItem* item : menuItems) {
			item->draw(*menuItemRenderer);
	}
}

#pragma once
#include <list>
#include "MenuItem.h"
#include "ResourceManager.h"
#include "Subject.h"
class Menu : public Subject
{
public:


	void addMenuItem(MenuItem* item) {
		menuItems.push_back(item);
	}

	void init();
	void moveSelectedUp();
	void moveSelectedDown();
	void select();
	void draw();


	MenuItem* selectedItem;

private:
	//THIS IS BAD
	SpriteRenderer* menuItemRenderer = new SpriteRenderer(ResourceManager::getShader("basic_sprite"));
	std::list<MenuItem*> menuItems;
};


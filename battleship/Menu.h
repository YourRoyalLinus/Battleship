#pragma once
#include <list>
#include "MenuItem.h"
#include "ResourceManager.h"
#include "Subject.h"
#include "UIComponent.h"
class Menu : public Subject
{
public:
	~Menu();

	void addMenuItem(MenuItem* item);
	void setTitle(UIComponent* title);

	void init();
	void moveSelectedUp();
	void moveSelectedDown();
	void update(float dt);
	void draw();


	MenuItem* selectedItem;

private:
	void moveCursorToSelectedItem();
	//THIS IS BAD
	SpriteRenderer* menuItemRenderer = new SpriteRenderer(ResourceManager::getShader("basic_sprite"));
	UIComponent* title = nullptr;
	UIComponent* cursor = new UIComponent(UIComponent::Alignment::NONE, ResourceManager::getTexture("cursor"), 1.0f);
	std::list<MenuItem*> menuItems;
};


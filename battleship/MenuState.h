#pragma once
#include "GameState.h"
#include <stack>
#include "Menu.h"
class MenuState : public GameState
{
public:
	MenuState(Game& game);
	void update() override;
	void render() override;

private:
	void selectMenuOption(MenuItem& menuItem);
};


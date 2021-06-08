#pragma once
#include "Command.h"
#include "InputHandler.h"
#include "Menu.h"
#include "MenuState.h"
class MenuCommand : public Command
{
public:
	MenuCommand(Menu& menu) : menu(menu) {/*...*/ }
	void execute() = 0;


protected:
	Menu& menu;
};

class MenuMoveUp : public MenuCommand
{
public:
	MenuMoveUp(Menu& menu) : MenuCommand(menu) {}
	void execute() override;
};

class MenuMoveDown : public MenuCommand
{
public:
	MenuMoveDown(Menu& menu) : MenuCommand(menu) {}
	void execute() override;
};

class MenuSelect : public MenuCommand
{
public:
	MenuSelect(Game& game, Menu& menu) : MenuCommand(menu), game(game) {}
	void execute() override;
private:
	Game& game;
};







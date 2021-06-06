#pragma once
#include "Command.h"
class InputHandler
{
public:
	static Command* handleInput();
	static void setMouseMove(Command* command) { mouseMove = command; }
	static void setLeftClick(Command* command) { leftClick = command; }
	static void setRightClick(Command* command) { rightClick = command; }

	static int mouseX, mouseY;

private:
	static Command* mouseMove;
	static Command* leftClick;
	static Command* rightClick;

};








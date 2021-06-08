#pragma once
#include "Command.h"
class InputHandler
{
public:
	enum class Key {
		UP,
		DOWN,
		ENTER
	};

	static Command* handleInput();
	static void setMouseMove(Command* command) { mouseMove = command; }
	static void setLeftClick(Command* command) { leftClick = command; }
	static void setRightClick(Command* command) { rightClick = command; }
	static void setKeyDown(Command* command) { keyDown = command; }
	static void setKeyUp(Command* command) { keyUp = command; }
	static void setEnterKey(Command* command) { enter = command; }

	static int mouseX, mouseY;

private:
	static Command* mouseMove;
	static Command* leftClick;
	static Command* rightClick;
	static Command* keyDown;
	static Command* keyUp;
	static Command* enter;

};








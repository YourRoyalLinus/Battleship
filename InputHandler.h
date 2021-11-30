#pragma once
#include "Event.h"
class InputHandler
{
public:
	enum class Key {
		UP,
		DOWN,
		ENTER
	};

	static Event* handleInput();
	static int mouseX, mouseY;

private:
};








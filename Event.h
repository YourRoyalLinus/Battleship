#pragma once
#include <utility>
class Event {
public:
	enum class Type {
		MOUSE_MOVE,
		LEFT_CLICK,
		RIGHT_CLICK,
		ARROW_DOWN,
		ARROW_UP,
		ENTER,
		DUPLICATE_GUESS,
		PLAYER_HIT,
		SHIP_SANK
	};

	Event(Type type) : eventType(type) {}

	Type eventType;
};

class MouseMove : public Event {
public:
	MouseMove(int x, int y) : Event(Type::MOUSE_MOVE), mouseX(x), mouseY(y) { /*...*/ }
	int mouseX, mouseY;
};

class LeftClick : public Event {
public:
	LeftClick(int x, int y) : Event(Type::LEFT_CLICK), mouseX(x), mouseY(y) { /*...*/ }
	int mouseX, mouseY;
};

class PlayerHit : public Event {
public:
	PlayerHit(std::pair<int, int> coord) : Event(Type::PLAYER_HIT), coord(coord) { /*...*/ }
	std::pair<int, int> coord;
};

class DuplicateGuess : public Event {
public: 
	DuplicateGuess(std::pair<int, int> coord) : Event(Type::DUPLICATE_GUESS), coord(coord){/*...*/ }
	std::pair<int, int> coord;
};



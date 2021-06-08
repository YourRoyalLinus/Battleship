#pragma once
class Event {
public:
	enum class Type {
		PLAYER_HIT,
		SHIP_SANK,
		MENU_SELECT
	};

	Event(Type type) : eventType(type) {}

	Type eventType;
};

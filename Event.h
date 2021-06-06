#pragma once
class Event {
public:
	enum class Type {
		PLAYER_HIT,
		SHIP_SANK
	};

	Type type;
};

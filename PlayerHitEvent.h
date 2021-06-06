#pragma once
#include "Event.h"
#include <utility>

class PlayerHitEvent : public Event {
public:
	PlayerHitEvent(std::pair<int,int> coords) : coords(coords)
	{
		this->type = Type::PLAYER_HIT;
	}

	std::pair<int, int> coords;
};

#pragma once
#include "Event.h"
#include <utility>

class PlayerHitEvent : public Event {
public:
	PlayerHitEvent(std::pair<int,int> coords) : Event(Event::Type::PLAYER_HIT), coords(coords) {}

	std::pair<int, int> coords;
};

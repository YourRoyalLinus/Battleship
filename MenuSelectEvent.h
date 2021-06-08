#pragma once
#include "Event.h"
#include "MenuItem.h"
class MenuSelectEvent : public Event {
public:
	MenuSelectEvent(MenuItem::ItemType type) : Event(Event::Type::MENU_SELECT), item(type) {}

	MenuItem::ItemType item;
};

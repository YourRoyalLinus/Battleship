#pragma once
#include "Command.h"
class PlaceShipCommand : public Command {
public:
	PlaceShipCommand(Player& player) : player(player) { /*...*/ }

	void execute() override {
		player.placeShip();
	}

private:
	Player& player;
};

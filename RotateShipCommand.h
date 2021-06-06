#pragma once
#include "Command.h"
class RotateShipCommand : public Command {
public:
	RotateShipCommand(Player& player) : player(player) { /*...*/ };

	void execute() override {
		this->player.shipToPlace->rotate();
	}

private:
	Player& player;
};

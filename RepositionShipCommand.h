#pragma once
#include "Command.h"
class RepositionShipCommand : public Command {
public:
	RepositionShipCommand(Player& player) : player(player) { /*...*/ }

	void execute() override;

private:
	Player& player;
};

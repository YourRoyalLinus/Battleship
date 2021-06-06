#pragma once
#include "Command.h"
#include "Game.h"
class MakeGuessCommand : public Command {
public:
	MakeGuessCommand(Game& game) : game(game) {/*...*/ };

	void execute() override {
		//wtf am I doing
		this->game.player->guess(*this->game.opponent);
		this->game.endTurn();
	}

private:
	Game& game;
};

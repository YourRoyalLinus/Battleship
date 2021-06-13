#pragma once
#include "GameState.h"
class MultiPlayerPlayState : public GameState
{
public:
	MultiPlayerPlayState(Game& game) : GameState(game) { /*...*/ }
	void update() override;
	void render() override;

private:
	//HACK
	bool guessedThisTurn = false;
};


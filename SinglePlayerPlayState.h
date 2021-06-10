#pragma once
#include "GameState.h"
class SinglePlayerPlayState : public GameState
{
public:
	SinglePlayerPlayState(Game& game) : GameState(game) {/*...*/ }
	void update() override;
	void render() override;
};


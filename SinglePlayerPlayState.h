#pragma once
#include "GameState.h"
class SinglePlayerPlayState : public GameState
{
	void update(Game& game) override;
	void render(Game& game) override;
};


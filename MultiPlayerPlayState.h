#pragma once
#include "GameState.h"
class MultiPlayerPlayState : public GameState
{
	void update(Game& game) override;
	void render(Game& game) override {}
};


#pragma once
#include "GameState.h"
class MultiPlayerSetup : public GameState
{
	void update(Game& game) override;
	void render(Game& game) override;
};


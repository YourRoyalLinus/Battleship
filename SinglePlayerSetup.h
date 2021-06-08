#pragma once
#include "GameState.h"
class SinglePlayerSetup : public GameState {
	void update(Game& game) override;
	void render(Game& game) override;
};

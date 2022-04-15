#pragma once
#include "GameState.h"
class PlayState : public GameState
{
public:
	PlayState(Game& game);
	virtual void update() = 0;
	void render();
};


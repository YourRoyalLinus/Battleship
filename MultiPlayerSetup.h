#pragma once
#include "GameState.h"
class MultiPlayerSetup : public GameState
{
public:
	MultiPlayerSetup(Game& game);
	void update() override;
	void render() override;
};


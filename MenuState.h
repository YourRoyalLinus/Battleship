#pragma once
#include "GameState.h"
#include <stack>
#include "Menu.h"
class MenuState : public GameState
{
public:
	void update(Game& game) override;
	void render(Game& game) override;
};


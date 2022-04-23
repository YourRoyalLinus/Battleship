#pragma once
#include "ResourceManager.h"
#include "UIComponent.h"
#include "PlayState.h"
class SinglePlayerPlayState : public PlayState
{
public:
	SinglePlayerPlayState(Game& game);
	void update() override;
private:
};


#pragma once
#include "SetupState.h"
class MultiPlayerSetup : public SetupState
{
public:
	MultiPlayerSetup(Game& game);
	void update() override;
};


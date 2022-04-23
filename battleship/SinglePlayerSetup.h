#pragma once
#include "SetupState.h"

class SinglePlayerSetup : public SetupState {
public:
	SinglePlayerSetup(Game& game);
	void update() override;
};

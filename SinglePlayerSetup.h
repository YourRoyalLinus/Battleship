#pragma once
#include "GameState.h"
#include "Player.h"
class SinglePlayerSetup : public GameState {
public:
	SinglePlayerSetup(Game& game) : GameState(game) {/*...*/ }
	void update() override;
	void render() override;
};

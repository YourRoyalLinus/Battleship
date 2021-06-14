#pragma once
#include "GameState.h"
#include "Player.h"
#include "UIComponent.h"
#include "ResourceManager.h"

class SinglePlayerSetup : public GameState {
public:
	SinglePlayerSetup(Game& game);
	void update() override;
	void render() override;

private:
	UIComponent text = UIComponent(UIComponent::Alignment::TOP_RIGHT, ResourceManager::getTexture("placeShips"), 1.0f);
};

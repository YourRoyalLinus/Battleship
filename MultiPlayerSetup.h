#pragma once
#include "GameState.h"
#include "UIComponent.h"
#include "ResourceManager.h"
class MultiPlayerSetup : public GameState
{
public:
	MultiPlayerSetup(Game& game);
	void update() override;
	void render() override;
private:
	UIComponent text = UIComponent(UIComponent::Alignment::TOP_RIGHT, ResourceManager::getTexture("placeShips"), 1.0f);
};


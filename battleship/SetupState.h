#pragma once
#include "GameState.h"
#include "UIComponent.h"
#include "ResourceManager.h"
class SetupState : public GameState
{
public:
	SetupState(Game& game);
	virtual void update() = 0;
	void render() override;

protected:
	UIComponent text = UIComponent(UIComponent::Alignment::TOP_RIGHT, ResourceManager::getTexture("placeShips"), 1.0f);
};


#pragma once
#include "GameState.h"
#include "ResourceManager.h"
#include "UIComponent.h"
class SinglePlayerPlayState : public GameState
{
public:
	SinglePlayerPlayState(Game& game);
	void update() override;
	void render() override;
private:
	//UIComponent turnPrompt = UIComponent(UIComponent::Alignment::TOP_LEFT, ResourceManager::getTexture("yourTurn"), 1.0f);
};


#pragma once
#include "GameState.h"
class MultiPlayerPlayState : public GameState
{
public:
	MultiPlayerPlayState(Game& game);
	void update() override;
	void render() override;

private:
	//HACK
	bool guessedThisTurn = false;
	/*UIComponent yourTurnPrompt = UIComponent(UIComponent::Alignment::TOP_LEFT, ResourceManager::getTexture("yourTurn"), 1.0f);
	UIComponent opponentTurnPrompt = UIComponent(UIComponent::Alignment::TOP_RIGHT, ResourceManager::getTexture("oppTurn"), 1.0f);*/


};


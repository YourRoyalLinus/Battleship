#pragma once
#include "GameState.h"
#include "UIComponent.h"
class ConnectingState : public GameState
{
public:
	ConnectingState(Game& game);
	void update() override;
	void render() override;
private:
	//THIS IS SO HACKY
	bool searching = true;
	UIComponent* text = nullptr;
	
};


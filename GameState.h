#pragma once
class Game;
class GameState {
public:
	virtual void update(Game& game) = 0;
};

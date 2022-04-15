#pragma once
class Game;
class GameState {
public:
	GameState(Game& game) : game(game) {/*...*/ }
	virtual void update() = 0;
	virtual void render() = 0;

protected:
	Game& game;
};

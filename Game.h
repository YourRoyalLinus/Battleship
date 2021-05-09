#pragma once
#include "Player.h"
#include "Board.h"
#include "SpriteRenderer.h"
#include "ParticleEmitter.h"
class Game
{
public:
	static const int SCREEN_WIDTH = 1200;
	static const int SCREEN_HEIGHT = 600;


	static const int SQUARE_PIXEL_SIZE = 75; //Magic numbers WOW!
	static const int SQUARE_PIXEL_HEIGHT = 75;


	enum class GameState {
		SETUP,
		PLAYING,
		OVER
	};

	GameState state;
	Player* human;
	Player* computer;
	Board* playerBoard;
	Board* radarBoard;

	Entity* grid;
	
	SpriteRenderer* waterRenderer;
	SpriteRenderer* spriteRenderer;
	SpriteRenderer* radarBoardRenderer;
	SpriteRenderer* gridRenderer;
	
	ParticleEmitter* fireEmitter, *smokeEmitter;

	bool humanTurn;

	Game();

	void init();
	void handleInput();
	void update(float dt);
	void render(float dt);


private:
	void updateShaders();
	//Returns seconds since epoch as float. Used to set uniforms.
	float mticks();
	//Input events we care about.
	int mousePosX, mousePosY;
	bool leftClick, rightClick;
	//Ship the palyer is placing;
	Ship* shipToPlace;

	std::vector<ParticleEmitter> fireEmitters;
	std::vector<ParticleEmitter> smokeEmitters;




};


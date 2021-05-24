#pragma once
#include "PeerNetwork.h"
#include "ConnectingPeer.h"
#include "HostingPeer.h"
#include "SpriteRenderer.h"
#include "Player.h"
#include "ComputerFactory.h"


#include "ParticleEmitter.h"
#include "PostProcessor.h"
class Game
{
	public:
	static const int SCREEN_WIDTH = 1200;
	static const int SCREEN_HEIGHT = 600;


	static const int SQUARE_PIXEL_SIZE = 75; //Magic numbers WOW!
	static const int SQUARE_PIXEL_HEIGHT = 75;

	PeerNetwork* net;
	GameParams::State state;
	GameParams::Turn turn;
	GameParams::Mode mode;
	Player* player;
	Opponent* opponent;

	Entity* grid;
	
	SpriteRenderer* waterRenderer;
	SpriteRenderer* spriteRenderer;
	SpriteRenderer* radarBoardRenderer;
	SpriteRenderer* gridRenderer;
	SpriteRenderer* shipRenderer;
	
	PostProcessor* effects;

	

	bool humanTurn;

	Game(Computer::Difficulty compDiff);
	Game(PeerNetwork* network);
	Game();
	~Game();

	void init();
	void handleInput();
	void update(float dt);
	void render(float dt);

	void changeDifficulty(Computer::Difficulty compDiff);
	//Static functions to load game?
	// PLACEHOLDER
	// PLACEHOLDER

private:
	void updateShaders();
	//Returns seconds since epoch as float. Used to set uniforms.
	float mticks();

	void renderRadarPings();

	//Spawn fire particle effect at specified square when the player gets hit
	void spawnFire(std::pair<int,int> square);
	//Screen shake post-processing effect that happens when the player gets hit
	void shakeScreen() {
		shakeTime = 0.2f;
		effects->shake = true;
	}
	//Remove fire emitters that were on a ship which sank
	void removeUnderwaterFire();

	//Input events we care about.
	int mousePosX, mousePosY;
	bool leftClick, rightClick;
	//Ship the palyer is placing;
	Ship* shipToPlace;

	//TODO: move this to a more sensible place.
	float shakeTime = 0.0f;

	std::vector<ParticleEmitter> fireEmitters;
	std::vector<ParticleEmitter> smokeEmitters;

};


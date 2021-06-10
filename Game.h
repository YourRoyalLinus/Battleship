#pragma once
#include "PeerNetwork.h"
#include "ConnectingPeer.h"
#include "HostingPeer.h"
#include "SpriteRenderer.h"
#include "Player.h"
#include "ParticleEmitter.h"
#include "PostProcessor.h"
#include "GameState.h"
#include "Observer.h"
#include "Menu.h"

class Game : public Observer
{
	public:

	PeerNetwork* net;
	GameParams::Mode mode;
	Player* player;
	Player* opponent;

	Entity* grid;
	Entity* menuWater;
	
	SpriteRenderer* waterRenderer;
	SpriteRenderer* spriteRenderer;
	SpriteRenderer* radarBoardRenderer;
	SpriteRenderer* gridRenderer;
	SpriteRenderer* shipRenderer;
	
	PostProcessor* effects;

	Player* activePlayer;
	Player* inactivePlayer;

	Game();
	~Game();


	GameState* state;

	std::stack<Menu*> menus;

	const std::string networkStartIp = "127.0.0.1"; //"67.248.183.2";

	void init();
	void update(float dt);
	void render(float dt);

	void onNotify(Event* event) override;

	//swap who's turn it is
	void endTurn() {
		Player* temp = activePlayer;
		activePlayer = inactivePlayer;
		inactivePlayer = temp;
	}

	//Spawn fire particle effect at specified square when the player gets hit
	void spawnFire(std::pair<int,int> square);
	//Screen shake post-processing effect that happens when the player gets hit
	void shakeScreen() {
		shakeTime = 0.2f;
		effects->shake = true;
	}

	//Returns seconds since epoch as float. Used to set uniforms.
	float mticks();
	
	void renderRadarPings();

	//Remove fire emitters that were on a ship which sank
	void removeUnderwaterFire();

	//Ship the palyer is placing;
	Ship* shipToPlace;

	//TODO: move this to a more sensible place.
	float shakeTime;

	std::vector<ParticleEmitter> fireEmitters;
	std::vector<ParticleEmitter> smokeEmitters;



};


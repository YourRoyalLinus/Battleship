#pragma once
#include "SpriteRenderer.h"
#include "Player.h"
#include "ParticleEmitter.h"
#include "PostProcessor.h"
#include "GameState.h"
#include "Observer.h"
#include "Menu.h"
#include "PeerNetwork.h"
#include "ConnectingPeer.h"
#include "HostingPeer.h"


class Game : public Observer
{
public:

	static const int SCREEN_WIDTH = 1200;
	static const int SCREEN_HEIGHT = 600;
	static const int SQUARE_PIXEL_SIZE = 75;
	static const int SQUARE_PIXEL_HEIGHT = 75;

	Game();
	~Game();

	PeerNetwork* net;
	Player* player, *opponent, *activePlayer, *inactivePlayer;
	Entity* grid, *menuWater;
	SpriteRenderer* waterRenderer, *spriteRenderer, *radarBoardRenderer, *gridRenderer, *shipRenderer;
	PostProcessor* effects;
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
	//Render radar hitmarkers to texture for radar frag shader to use
	void renderRadarPings();
	//Remove fire emitters that were on a ship which sank
	void removeUnderwaterFire();

	//TODO: move this to a more sensible place.
	float shakeTime;
	std::vector<ParticleEmitter> fireEmitters;
	std::vector<ParticleEmitter> smokeEmitters;

};


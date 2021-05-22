#pragma once
#include "PeerNetwork.h"
#include "ConnectingPeer.h"
#include "HostingPeer.h"
#include "SpriteRenderer.h"
#include "Player.h"
#include "ComputerFactory.h"


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

		SpriteRenderer* playerBoardRenderer;
		SpriteRenderer* spriteRenderer;
		SpriteRenderer* radarBoardRenderer;

		Game(Computer::Difficulty compDiff);
		Game(PeerNetwork* network);
		Game();
		~Game();

		void init();
		void handleInput();
		void update();
		void render();

		void changeDifficulty(Computer::Difficulty compDiff);
		//Static functions to load game?
		// PLACEHOLDER
		// PLACEHOLDER

	private:
		void updateShaders();
		//Returns seconds since epoch as float. Used to set uniforms.
		float mticks();
		//Input events we care about.
		int mousePosX, mousePosY;
		bool leftClick, rightClick;
		//Ship the palyer is placing;
		Ship* shipToPlace;
};


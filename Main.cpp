#pragma once
#include <algorithm>
#include <ctime>
//#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stb_image.h>
#include "HostingPeer.h"
#include "ConnectingPeer.h"
#include "Texture2D.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "Board.h"
#include "Player.h"
#include "Ship.h"
#include "Game.h"
#include "Shader.h"


//TODOS:
//CLEAN UP BOARD CONSTRUCTOR
//MAKE PRETTY GRAPHICS
//PARTICLES?
//MAKE SHIP DRAWING LESS SCUFFED
//Make it so if hotloaded shader failed to compile, old shader is used.
//MAKE NAMING CONSISTENT FOR EVERYTHING.
//CLEAN UP MEMORY CORRECTLY
//PUT CONSTANT VALUES IN ONE PLACE & WHERE THEY SHOULD BE (Config file we initialize when game starts?)
//GENERAL REFACTORING / CLEANUP / ABSTRACTING 


//The window we'll be rendering to
SDL_Window* window = NULL;

//OpenGL context
SDL_GLContext gContext;

//prototype
void teardown();

//Initialize SDL, OpenGL & GLAD.
void init() {

	constexpr int WIDTH = Game::SCREEN_WIDTH;
	constexpr int HEIGHT = Game::SCREEN_HEIGHT;

	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
	}
	SDL_GL_LoadLibrary(NULL);

	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 6 );

	window = SDL_CreateWindow( "BATTLESHIP", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if( window == NULL ){
		printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
	}
	
	//Create context
	gContext = SDL_GL_CreateContext( window );
	if( gContext == NULL ){
		printf( "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError() );
	}
	
	// Check OpenGL properties
	printf("OpenGL loaded\n");
	if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
		printf("Failed to initialize GLAD!");
		exit(-2);
	}

	printf("Vendor:   %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version:  %s\n", glGetString(GL_VERSION));


	//Use Vsync
	if( SDL_GL_SetSwapInterval( 1 ) < 0 )
	{
		printf( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
	}

	// Disable depth test and face culling.
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

	glViewport(0, 0, WIDTH, HEIGHT);
}

float ticks()
{
    typedef std::chrono::high_resolution_clock clock;
    typedef std::chrono::duration<float, std::milli> duration;

    static clock::time_point start = clock::now();
    duration elapsed = clock::now() - start;
    return elapsed.count() / 1000.0;
}

const int FRAME_RATE = 60;
const int FRAMETIME = 1000 / FRAME_RATE;

int main(int argc, char* argv[]) {

	//This is how I'm doing RNG for now!
	srand((unsigned)time(NULL));

	//Select Game Mode
	PeerNetwork* net = new PeerNetwork();
	const std::string networkStartIp = "67.248.183.2";

	std::string input;
	GameParams::Mode mode;
	Computer::Difficulty compDiff;
	std::cout << "Select a game mode:\n1. Player vs Player\n2. Computer\n";
	std::cin >> input;

	switch (stoi(input)) {
		case 1://PLAYER		
			mode = GameParams::Mode::PVP;
			std::cout << "\nFind an opponent:\n1. Random opponent\n2. Challenge a friend\n";
			std::cin >> input;
			switch (stoi(input)) {
				case 1: //Random Opponent
					net->ConnectToRandomOpponent(networkStartIp);
					break;
				case 2: // TODO Challenge a friend
					std::cout << "\nCURRENTLY NOT IMPLEMENTED"; //TODO Connect via game code (Hashed IP?)
					exit(0);
					break;
			}
			break;
		case 2: //COMPUTER
			mode = (GameParams::Mode::SOLO);
			std::cout << "\nSelect Computer Difficulty:\n1. EASY\n2. MEDIUM (Default)\n3. HARD\n";
			std::cin >> input;
			switch (stoi(input)) {
				case 1:
					compDiff = Computer::Difficulty::EASY;
					break;
				case 2:
					compDiff = Computer::Difficulty::MEDIUM;
					break;
				case 3:
					compDiff = Computer::Difficulty::HARD;
					break;
				default:
					compDiff = Computer::Difficulty::MEDIUM;
					break;
			}
			break;
		default: //COMPUTER
			mode = (GameParams::Mode::SOLO);
			compDiff = Computer::Difficulty::MEDIUM;
			break;
	}

	//Initialize SDL/OpenGL
	init();
	Game *game;

	if (mode == GameParams::Mode::SOLO) {
		game = new Game();
		game->init();
		if (compDiff != Computer::Difficulty::MEDIUM) {
			game->changeDifficulty(compDiff);
		}
	}
	else if (mode == GameParams::Mode::PVP) { 
		game = new Game(net);

		//Randomize which player goes first
		//TODO Need a way to display who's turn it is
		if (game->net->peerType == Peer::PeerType::HOSTING_PEER) {
			int first = rand() % 100;
			bool turn = (first % 2 == 0);
			int resp = game->net->SendTurn(turn);
			if (resp == 0) { 
				std::cout << "Opposing player disconnected...\n";
				exit(-1);
			}

			if (turn) {
				game->turn = GameParams::Turn::PLAYER;
			}
			else {
				game->turn = GameParams::Turn::OPPONENT;
			}
		}
		else if (game->net->peerType == Peer::PeerType::CONNECTING_PEER) {
			auto buffer = game->net->ReceiveData();
			if (buffer.bufferType == Payload::BufferType::TURN) {
				if (buffer.yourTurn) {
					game->turn = GameParams::Turn::PLAYER;
				}
				else {
					game->turn = GameParams::Turn::OPPONENT;
				}
			}
			else {
				std::cout << "Unable to start game. Disconnecting...";
				exit(-1);
			}
		}

		game->init();
	}



	while (game->state != GameParams::State::OVER) {
	
		game->handleInput();
		game->update();
		
		//render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		game->render();

			SDL_GL_SwapWindow(window);
			deltaTime = 0;
		

		float framEnd = ticks();
		deltaTime += framEnd - frameStart;
		frameStart = framEnd;
	}
	
	teardown();

	return 0;
	
}

void teardown() {
	
	IMG_Quit();
	SDL_Quit();
}







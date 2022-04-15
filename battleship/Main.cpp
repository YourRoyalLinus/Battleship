#pragma once
#include <algorithm>
#include <ctime>
#include "Game.h"
//#include <SDL_opengl.h>
#include <SDL_image.h>


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


	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
	}
	SDL_GL_LoadLibrary(NULL);

	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 6 );

	window = SDL_CreateWindow( "BATTLESHIP", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
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

	glViewport(0, 0, Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT);
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


	//Initialize SDL/OpenGL
	init();
	Game *game = new Game();

	game->init();


	float deltaTime = 0;
	float frameStart = ticks();
	while (true) {
		
		game->update(deltaTime);
		
		//render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		game->render(deltaTime);

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
	
	SDL_Quit();
}







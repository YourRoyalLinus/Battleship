#include <SDL.h>
#include <SDL_image.h>
#include "Board.h"

//TODO: !!!!!!! THIS SDL CODE IS ROUGH GETTING IT WORKING STUFF! MAKE THIS LESS TRASH LATER !!!!!!!!!


//Global Window context
SDL_Window* gWindow = NULL;
//Global Renderer context
SDL_Renderer* gRenderer = NULL;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;

enum class GameState {
	OVER,
	PLAYING
};

GameState state;

//TODO: Seperate this into seperate interface?

void InitSDL() {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
		throw("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());


	//Set texture filtering to linear
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		throw("Warning: Linear texture filtering not enabled!");

	//Create window
	gWindow = SDL_CreateWindow("Battleship!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow == NULL)
		throw("Window could not be created! SDL_Error: %s\n", SDL_GetError());


	//Create renderer for window
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (gRenderer == NULL)
		throw("Renderer could not be created! SDL Error: %s\n", SDL_GetError());


	//Initialize renderer color
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF); //White

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
		throw("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());

}

SDL_Texture* loadTextureFromFile(std::string filePath) {
	//Create a surface from the given path
	SDL_Surface* loadedSurface = IMG_Load(filePath.c_str());
	if (loadedSurface == NULL)
		printf("Failed to load texture from: %s\n", filePath.c_str());
		

	//Generate the Texture from our surface.
	SDL_Texture* loadedTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);

	if (loadedTexture == NULL)
		printf("Failed to create texture from: %s : %s", filePath.c_str(), SDL_GetError());

	SDL_FreeSurface(loadedSurface);

	return loadedTexture;
}

void teardown(std::vector<SDL_Texture*> textures) {
	//Be a good citizen
	for (auto texture : textures)
		SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* argv[]) {

	//Initialize SDL
	InitSDL();

	std::vector<SDL_Texture*> textures;

	//Grab Grid Texture
	SDL_Texture* gridTexture = loadTextureFromFile("Textures\\BattleshipGrid.png");
	SDL_Texture* shipTexture = loadTextureFromFile("Textures\\Ship.png");

	//Add textures to vector for teardown later
	textures.push_back(gridTexture);
	textures.push_back(shipTexture);

	//Make a board
	Board board;

	//Put a ship down on it.
	Ship ship(Ship::Type::BATTLESHIP);
	using Coord = std::pair<int, int>;
	Coord c1(4, 4);
	Coord c2(4, 5);
	Coord c3(4, 6);
	Coord c4(4, 7);
	std::vector<Coord> coords = { c1,c2,c3,c4 };
	board.placeShip(ship, coords);

	std::vector<Square> occupiedSquares = board.occupiedSquares();
	for (auto square : occupiedSquares)
		std::cout << square.col << " " << square.row << std::endl;

	state = GameState::PLAYING; //Start the game
	SDL_Event event; //input event

	while (state != GameState::OVER) {
		//Handle events on queue
		while (SDL_PollEvent(&event) != 0)
		{
			//User requests quit
			if (event.type == SDL_QUIT) //If you 'X' out
			{
				state = GameState::OVER;
			}
		}

		SDL_RenderClear(gRenderer);
		//Draw the Grid
		SDL_RenderCopy(gRenderer, gridTexture,0,0);
		//Draw the Pieces
		for (auto square : board.occupiedSquares()) {
			/* SDL_Rect = { x_pos, y_pos, width, height } */
			/* The x and y parameters of the rectange seem strange but it's because the "col" is how far across you are; ie 'x', the row is how far down you are ie 'y'*/
			SDL_Rect dest = { square.col * 100, square.row * 100, 100, 100}; //Magic Numbers! Wow!
			SDL_RenderCopy(gRenderer, shipTexture, 0, &dest);
		}
	
		SDL_RenderPresent(gRenderer);
	}
	
	teardown(textures);

	return 0;
	
}

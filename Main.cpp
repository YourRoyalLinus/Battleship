#include <SDL.h>
#include <SDL_image.h>
#include "Board.h"
#include "Player.h"
#include <algorithm>
#include "Renderer.h"


//TODO: LEAKING MEMORY RIGHT NOW FROM EVERYWHERE THAT USES A TEXTURE!!!!!! UH OH!!!! Implement destructors

const int SQUARE_PIXEL_SIZE = 100;

enum class GameState {
	SETUP,
	PLAYING,
	OVER
};

GameState state;


void teardown(std::vector<SDL_Texture*> textures) {
	//Be a good citizen
	for (auto texture : textures)
		SDL_DestroyTexture(texture);
	Renderer::cleanup();
	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* argv[]) {

	//Initialize SDL
	Renderer::init();

	std::vector<SDL_Texture*> textures;
	

	//Make human player
	Player human;
	//Make their board
	Board board;


	state = GameState::SETUP;
	SDL_Event event; //input event

	while (state == GameState::SETUP) {
		
		if (human.ships.empty()) {
			state = GameState::PLAYING;
			break;
		}

		Ship& currentShipRef = human.ships.back();
		
		int mouseXSquare, mouseYSquare; //what square is the mouse currently in.

		while (SDL_PollEvent(&event) != 0) {
			Renderer::clear();
			//Draw the Grid

			board.draw();

			if (event.type == SDL_QUIT) {
				exit(-1);
			}
			if (event.type == SDL_MOUSEMOTION) {
				SDL_GetMouseState(&mouseXSquare, &mouseYSquare);

				/* Clamp position to grid square size */
				mouseXSquare /= SQUARE_PIXEL_SIZE;
				mouseYSquare /= SQUARE_PIXEL_SIZE;

				currentShipRef.snapToPosition({ mouseYSquare, mouseXSquare }); //Ypos = row, Xpos = col.
				currentShipRef.draw();
			}
			if (event.type == SDL_MOUSEBUTTONDOWN) {
				if (event.button.button == SDL_BUTTON_LEFT) { //button.button ???? OK
					if (board.placeShip(currentShipRef, currentShipRef.coords))
					{
						human.ships.pop_back();
					}
					else {
						printf("You can't place a ship there!\n");
					}
				}
				else if (event.button.button == SDL_BUTTON_RIGHT) {
					currentShipRef.rotate();
					currentShipRef.draw();
				}

			}

			Renderer::present();
		}
	}

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

		Renderer::clear();
		
		board.draw();
	
		Renderer::present();
	}
	
	teardown(textures);

	return 0;
	
}

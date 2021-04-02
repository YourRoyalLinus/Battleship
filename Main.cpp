#include <SDL.h>
#include <SDL_image.h>
#include "Board.h"
#include "Player.h"
#include <algorithm>
#include "Renderer.h"
#include <ctime>
#include "PlayerBoard.h"
#include "RadarBoard.h"


//TODO: LEAKING MEMORY RIGHT NOW FROM EVERYWHERE THAT USES A TEXTURE!!!!!! UH OH!!!! Implement destructors

const int SQUARE_PIXEL_WIDTH = 75; //Magic numbers WOW!
const int SQUARE_PIXEL_HEIGHT = 75;


enum class GameState {
	SETUP,
	PLAYING,
	OVER
};

GameState state;
bool humanTurn = true;

void teardown() {
	
	Renderer::cleanup();
	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* argv[]) {

	//This is how I'm doing RNG for now!
	srand((unsigned)time(NULL));

	//Initialize SDL
	Renderer::init();

	
	//Make human player
	Player human;
	//Make Neural Net Artificial Intelligence Blockchain Smart BattleAI9000
	Player computer;
	//Make human player's board
	PlayerBoard humanBoard;
	//Computer board which looks like a radar board to the player
	RadarBoard computerBoard;
	

	state = GameState::SETUP;

	SDL_Event event; //input event

	while (state == GameState::SETUP) {

		if (humanTurn) {

			if (human.ships.empty()) {
				//state = GameState::PLAYING;
				humanTurn = false;
				continue;
			}

			Ship& currentShipRef = human.ships.back();

			int mouseXSquare, mouseYSquare; //what square is the mouse currently in.

			while (SDL_PollEvent(&event) != 0) {
				Renderer::clear();
				
				computerBoard.draw();
				humanBoard.draw();

				if (event.type == SDL_QUIT) {
					exit(-1);
				}
				if (event.type == SDL_MOUSEMOTION) {
					SDL_GetMouseState(&mouseXSquare, &mouseYSquare);
					/* Stupid hack!!! adjust the mouse by setup board position offset */
					mouseXSquare -= Renderer::GUESS_BOARD_VIEWPORT.w;
					/* Clamp position to grid square size */
					mouseXSquare /= SQUARE_PIXEL_WIDTH;
					mouseYSquare /= SQUARE_PIXEL_HEIGHT;

					currentShipRef.snapToPosition({ mouseYSquare, mouseXSquare }); //Ypos = row, Xpos = col.
					currentShipRef.draw();
				}
				if (event.type == SDL_MOUSEBUTTONDOWN) {
					if (event.button.button == SDL_BUTTON_LEFT) { //button.button ???? OK
						if (humanBoard.placeShip(currentShipRef, currentShipRef.coords))
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

		//TODO: THIS IS GARBAGE. Make this more sophisticaed and there is some abstraction to be done.
		if (!humanTurn) {
			while (!computer.ships.empty()) {
				Ship& computerShipRef = computer.ships.back();
				int row = rand() % 8;
				int col = rand() % 8;
				
				computerShipRef.snapToPosition({ row, col });

				if (computerBoard.placeShip(computerShipRef, computerShipRef.coords)) {
					computer.ships.pop_back();
				}
			}
			computerBoard.print();
			state = GameState::PLAYING;
			humanTurn = true;
		}

	}
	
	while (state == GameState::PLAYING) {

		if (humanTurn) {
			//std::cout << "Guess a ship position!" << std::endl;
			//Handle events on queue
			while (SDL_PollEvent(&event) != 0)
			{
				//User requests quit
				if (event.type == SDL_QUIT) //If you 'X' out
				{
					state = GameState::OVER;
				}

				if (event.type == SDL_MOUSEBUTTONDOWN) {

					if (event.button.button == SDL_BUTTON_LEFT) {
						int mouseXSquare, mouseYSquare; //what square is the mouse currently in.

						SDL_GetMouseState(&mouseXSquare, &mouseYSquare);

						/* Clamp position to grid square size */
						mouseXSquare /= SQUARE_PIXEL_WIDTH;
						mouseYSquare /= SQUARE_PIXEL_HEIGHT;

						std::cout << "You guessed ROW: " << mouseYSquare << "COL: " << mouseXSquare << std::endl;

						if (computerBoard.guess({ mouseYSquare, mouseXSquare })) //Again Y = ROW, X = COL!
							std::cout << "Human Player HIT!!!!!" << std::endl;
						/*else
							std::cout << "Human player MISSED!" << std::endl;*/

						humanTurn = false;
					}
				}
			}

		}

		if (!humanTurn) {
			int row = rand() % 8;
			int col = rand() % 8;

			std::cout << "Computer player guessed  ROW: " << row << " COL: " << col << std::endl;

			humanBoard.guess({ row, col });
		
			humanTurn = true;

		}
		

		Renderer::clear();
		
		computerBoard.draw();
		humanBoard.draw();
	
		Renderer::present();

		if (humanBoard.activeShips.empty()) {
			state = GameState::OVER;
			std::cout << "GAME OVER! COMPUTER WON! YOU SUCK!" << std::endl;
		}
		else if (computerBoard.activeShips.empty()) {
			state = GameState::OVER;
			std::cout << "GAME OVER! YOU WON!" << std::endl;
		}
	}
	
	teardown();

	return 0;
	
}

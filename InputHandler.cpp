#include "InputHandler.h"
#include <SDL.h>

int InputHandler::mouseX = 0;
int InputHandler::mouseY = 0;
Command* InputHandler::mouseMove = nullptr;
Command* InputHandler::leftClick = nullptr;
Command* InputHandler::rightClick = nullptr;


Command* InputHandler::handleInput() {
	SDL_Event event;
	if (SDL_PollEvent(&event) != NULL) {
		if (event.type == SDL_QUIT) {
			exit(0);
		}
		else if (event.type == SDL_MOUSEBUTTONDOWN) {
			if (event.button.button == SDL_BUTTON_LEFT) {
				return leftClick;
			}
			else if(event.button.button == SDL_BUTTON_RIGHT) {
				return rightClick;
			}	
		}
		else if (event.type == SDL_MOUSEMOTION) {
			SDL_GetMouseState(&mouseX, &mouseY);
			return mouseMove;
		}
		else {
			return nullptr;
		}
	}
}

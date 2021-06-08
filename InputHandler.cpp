#include "InputHandler.h"
#include <SDL.h>

int InputHandler::mouseX = 0;
int InputHandler::mouseY = 0;
Command* InputHandler::mouseMove = nullptr;
Command* InputHandler::leftClick = nullptr;
Command* InputHandler::rightClick = nullptr;
Command* InputHandler::keyUp = nullptr;
Command* InputHandler::keyDown = nullptr;
Command* InputHandler::enter = nullptr;


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
		else if(event.type == SDL_KEYDOWN){
			if (event.key.keysym.sym == SDLK_DOWN) {
				return keyDown;
			}
			else if (event.key.keysym.sym == SDLK_UP) {
				return keyUp;
			}
			else if (event.key.keysym.sym == SDLK_RETURN) {
				return enter;
			}
			else {
				return nullptr;
			}
		}
			else {
			return nullptr;
		}
	}
}

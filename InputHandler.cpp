#include "InputHandler.h"
#include <SDL.h>

int InputHandler::mouseX = 0;
int InputHandler::mouseY = 0;


Event* InputHandler::handleInput() {
	SDL_Event event;
	if (SDL_PollEvent(&event) != NULL) {
		if (event.type == SDL_QUIT) {
			exit(0);
		}
		else if (event.type == SDL_MOUSEBUTTONDOWN) {
			if (event.button.button == SDL_BUTTON_LEFT) {
				SDL_GetMouseState(&mouseX, &mouseY);
				return new LeftClick(mouseX, mouseY);
			}
			else if(event.button.button == SDL_BUTTON_RIGHT) {
				return new Event(Event::Type::RIGHT_CLICK);
			}	
		}
		else if (event.type == SDL_MOUSEMOTION) {
			SDL_GetMouseState(&mouseX, &mouseY);
			return new MouseMove(mouseX, mouseY);
		}
		else if(event.type == SDL_KEYDOWN){
			if (event.key.keysym.sym == SDLK_DOWN) {
				return new Event(Event::Type::ARROW_DOWN);
			}
			else if (event.key.keysym.sym == SDLK_UP) {
				return new Event(Event::Type::ARROW_UP);
			}
			else if (event.key.keysym.sym == SDLK_RETURN) {
				return new Event(Event::Type::ENTER);
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

#include "Renderer.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

SDL_Window* Renderer::window = nullptr;
SDL_Renderer* Renderer::renderer = nullptr;


const SDL_Rect Renderer::CHAT_VIEWPORT = { 0, 0, 200, SCREEN_HEIGHT };
const SDL_Rect Renderer::GUESS_BOARD_VIEWPORT = { CHAT_VIEWPORT.w, 0, (SCREEN_WIDTH - 200) / 2, SCREEN_HEIGHT };
const SDL_Rect Renderer::PLACE_BOARD_VIEWPORT = { GUESS_BOARD_VIEWPORT.x + GUESS_BOARD_VIEWPORT.w , 0, GUESS_BOARD_VIEWPORT.w, SCREEN_HEIGHT };


void Renderer::init() {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
		throw("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());


	//Set texture filtering to linear
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		throw("Warning: Linear texture filtering not enabled!");

	//Create window
	window = SDL_CreateWindow("Battleship!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL)
		throw("Window could not be created! SDL_Error: %s\n", SDL_GetError());


	//Create renderer for window
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL)
		throw("Renderer could not be created! SDL Error: %s\n", SDL_GetError());


	//Initialize renderer color
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF); //White

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
		throw("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());

	if (TTF_Init() == -1) {
		//throw("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		exit(-1);
	}


}

void Renderer::render(Texture& texture, const SDL_Rect* clip,const SDL_Rect* dest) {
	SDL_RenderCopy(renderer, texture.texture, clip, dest);
}

void Renderer::renderEx(Texture& texture, const SDL_Rect* clip, const SDL_Rect* dest, const double angle, const SDL_Point* center, const SDL_RendererFlip flip){
	SDL_RenderCopyEx(renderer, texture.texture, clip, dest, angle, center, flip);
}

void Renderer::clear() {
	SDL_RenderClear(renderer);
}

void Renderer::present() {
	SDL_RenderPresent(renderer);
}

void Renderer::setViewPort(const SDL_Rect* viewport) {
	SDL_RenderSetViewport(renderer, viewport);
}

void Renderer::cleanup() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}
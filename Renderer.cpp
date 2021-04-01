#include "Renderer.h"
#include <SDL.h>
#include <SDL_image.h>

SDL_Window* Renderer::window = nullptr;
SDL_Renderer* Renderer::renderer = nullptr;

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

}

SDL_Texture* Renderer::loadTextureFromFile(std::string filePath) {
	//Create a surface from the given path
	SDL_Surface* loadedSurface = IMG_Load(filePath.c_str());
	if (loadedSurface == NULL)
		printf("Failed to load texture from: %s\n", filePath.c_str());


	//Generate the Texture from our surface.
	SDL_Texture* loadedTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);

	if (loadedTexture == NULL)
		printf("Failed to create texture from: %s : %s", filePath.c_str(), SDL_GetError());

	SDL_FreeSurface(loadedSurface);

	return loadedTexture;
}

void Renderer::render(SDL_Texture* texture, const SDL_Rect* clip,const SDL_Rect* dest) {
	SDL_RenderCopy(renderer, texture, clip, dest);
}
void Renderer::clear() {
	SDL_RenderClear(renderer);
}

void Renderer::present() {
	SDL_RenderPresent(renderer);
}

void Renderer::cleanup() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}
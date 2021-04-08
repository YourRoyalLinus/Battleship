#pragma once
#include <memory>
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <SDL_ttf.h>
#include "Texture.h"
class Renderer
{
public:
	static const int SCREEN_WIDTH = 1400;
	static const int SCREEN_HEIGHT = 600;

	static const SDL_Rect GUESS_BOARD_VIEWPORT;
	static const SDL_Rect PLACE_BOARD_VIEWPORT;
	static const SDL_Rect CHAT_VIEWPORT;
	static SDL_Rect CURRENT_CHAT_LINE;

	static void init();
	static void cleanup();
	static void clear();
	/* Render a Texture at a given location
	   clip = SDL_Rect describing source on sprite sheet
	   dest = SDL_Rect describing render destination */
	static void render(Texture& texture, const SDL_Rect* clip = 0, const SDL_Rect* dest = 0);
	static void renderEx(Texture& texture, const SDL_Rect* clip = 0, const SDL_Rect* dest = 0, const double angle = 0.0, const SDL_Point* center = 0, const SDL_RendererFlip flip = SDL_FLIP_NONE);
	static void present();
	static void setViewPort(const SDL_Rect* viewport = 0);
	

	static SDL_Renderer* renderer;
	
private:
	Renderer();
	static SDL_Window* window;
};


#pragma once
#include <memory>
#include <SDL.h>
#include <SDL_image.h>
#include <string>
class Renderer
{
public:
	static const int SCREEN_WIDTH = 800;
	static const int SCREEN_HEIGHT = 800;

	static void init();
	static SDL_Texture* loadTextureFromFile(std::string filePath);
	static void cleanup();
	static void clear();
	/* Render a Texture at a given location
	   clip = SDL_Rect describing source on sprite sheet
	   dest = SDL_Rect describing render destination */
	static void render(SDL_Texture* texture, const SDL_Rect* clip = 0, const SDL_Rect* dest = 0);
	static void present();
	

	
private:
	Renderer();
	static SDL_Window* window;
	static SDL_Renderer* renderer;
};


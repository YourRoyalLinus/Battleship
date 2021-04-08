#include "Texture.h"
#include <SDL_image.h>
#include "Renderer.h"



Texture::~Texture() {
	free();
}

//Loads image at specified path
void Texture::loadFromFile(std::string path) {

	free();
	
	//Create a surface from the given path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
		printf("Failed to load texture from: %s\n", path.c_str());


	//Generate the Texture from our surface.
	texture = SDL_CreateTextureFromSurface(Renderer::renderer, loadedSurface);

	if ( texture == NULL)
		printf("Failed to create texture from: %s : %s", path.c_str(), SDL_GetError());

	width = loadedSurface->w;
	height = loadedSurface->h;

	SDL_FreeSurface(loadedSurface);
}



//Deallocates texture
void Texture::free() {
	if (texture != nullptr) {
		SDL_DestroyTexture(texture);
		texture = nullptr;
		width = 0;
		height = 0;
	}
}

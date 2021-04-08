#pragma once
#include <SDL.h>
#include <string>
class Texture
{
public:

	Texture(): width(0), height(0), texture(nullptr) {}

	Texture(std::string path) : Texture() {
		loadFromFile(path);
	}
	//Deallocates memory
	~Texture();

	//Loads image at specified path
	void loadFromFile(std::string path);


	//Deallocates texture
	void free();

	//Image Dimensions
	int width;
	int height;

	SDL_Texture* texture;
};


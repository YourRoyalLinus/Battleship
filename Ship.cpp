#include "Ship.h"
#include <algorithm>
#include <utility>
#include "Renderer.h"

const std::string Ship::imgPath = "Textures\\Ship.png";

Ship::Ship(Type type): type(type), length(sizes[type]), rotated(false), hitsTaken(0) {
	texture = Renderer::loadTextureFromFile(imgPath);
	for (int i = 0; i <= length; i++) {
		coords.push_back({ 0,i });
	}
}

/* Utility to make moving coords easier */
std::pair<int, int> operator+(std::pair<int, int>& lhs, const std::pair<int, int>& rhs) {
	return std::make_pair(lhs.first + rhs.first, lhs.second + rhs.second);
}


void Ship::snapToPosition(std::pair<int,int> position) {
	coords.clear();
	for (int i = 0; i < length; i++) {
		if (rotated)
			coords.push_back(position + std::make_pair(i, 0));
		else
			coords.push_back(position + std::make_pair(0, i));
	}
}

void Ship::draw() {
	for (auto coord : coords) {
		SDL_Rect dest = { coord.second * 75, coord.first * 75, 75, 75 }; //Magic Numbers because each grid cell is 100 by 100 pixels right now
		Renderer::render(texture, 0, &dest);
	}
}

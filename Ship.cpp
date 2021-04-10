#include "Ship.h"
#include <algorithm>
#include <utility>
#include "Board.h"


Ship::Ship(Type type): type(type),length(sizes[type]), orientation(Orientation::LEFT), hitsTaken(0), imgPath(images[type]) {
	//texture.reset(new Texture(imgPath));
	for (int i = 0; i <= length; i++) {
		coords.push_back({ 0,i });
	}
}



/* Utility to make moving coords easier */
std::pair<int, int> operator+(const std::pair<int, int>& lhs, const std::pair<int, int>& rhs) {
	return std::make_pair(lhs.first + rhs.first, lhs.second + rhs.second);
}

std::pair<int, int> operator-(const std::pair<int, int>& lhs, const std::pair<int, int>& rhs) {
	return std::make_pair(lhs.first - rhs.first, lhs.second - rhs.second);
}

void Ship::snapToPosition(std::pair<int,int> position) {
	coords.clear();
	for (int i = 0; i < length; i++) {
		// Here the orientation refers to where the head of the ship points. So if the orientation is LEFT for example the rest of the ship expands to the right, etc.
		// TODO:This is pretty confusing and bad I should probably change this later
		switch (orientation) {
		case Orientation::LEFT:
			coords.push_back(position + std::make_pair(0, i));
			break;
		case Orientation::UP:
			coords.push_back(position + std::make_pair(i, 0));
			break;
		case Orientation::RIGHT:
			coords.push_back(position + std::make_pair(0, -i));
			break;
		case Orientation::DOWN:
			coords.push_back(position + std::make_pair(-i, 0));
			break;
		}
	}
}

void Ship::draw() {
	//constexpr auto SQ_SIZE = Board::SQUARE_PIXEL_SIZE;

	//SDL_Rect dest = { coords[0].second * SQ_SIZE, coords[0].first * SQ_SIZE, SQ_SIZE * length, SQ_SIZE };
	//SDL_Point center = { SQ_SIZE/2, SQ_SIZE/2 }; //Middle of a square
	//Renderer::renderEx(*texture, 0, &dest, static_cast<int>(orientation) * 90, &center);

	
}

void Ship::rotate() {
	std::pair<int, int> pivot = coords[0];
	std::vector<std::pair<int, int>> relativeCoords = getCoordsRelativeToPivot(pivot);
	std::pair<std::pair<int, int>, std::pair<int, int>> rotationMatrix = { { 0 , 1}, { -1, 0 } };
	for (auto& coord : relativeCoords) {
		auto localCoord = vectorMatrixProduct2(coord, rotationMatrix);
		coord = localCoord + pivot;
	}
	coords = relativeCoords;
	orientation = static_cast<Orientation>((static_cast<int>(orientation) + 1) % 4);
}

//TODO: This bit of linear algebra code can probably be put somewhere else but the ship is the only thing in the game that rotates so for now its here.
std::vector<std::pair<int, int>> Ship::getCoordsRelativeToPivot(std::pair<int,int> pivot) {
	std::vector<std::pair<int, int>> result;
	for (const auto coord : coords) {
		result.push_back(coord - pivot);
	}
	return result;
}
int Ship::dotPoduct2(std::pair<int, int> vec1, std::pair<int, int> vec2) {
	return vec1.first * vec1.second + vec1.second * vec2.second;
}
std::pair<int, int> Ship::vectorMatrixProduct2(std::pair<int, int> vec, std::pair<std::pair<int, int>, std::pair<int, int>> mat) {
	std::pair<int, int> result;
	//dumb
	result.first = dotPoduct2(mat.first, vec);
	result.second = dotPoduct2(mat.second, vec);
	return result;
}
	

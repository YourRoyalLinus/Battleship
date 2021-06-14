#pragma once
#include <algorithm>
#include "ResourceManager.h"
#include "Board.h"
#include "Ship.h"
#include "ResourceManager.h"
#include <glm/glm.hpp>


Ship::Ship(Type type): type(type), length(sizes[type]), orientation(Orientation::LEFT), hitsTaken(0) {
	//Entity gfx stuff
	std::string shipName = sprites[type];
	sprite = ResourceManager::getTexture(sprites[type]);
	size = glm::vec2(length * Board::SQUARE_PIXEL_SIZE, Board::SQUARE_PIXEL_SIZE);
	tex2 = ResourceManager::getTexture("sunGradient");


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

void Ship::snapToPosition(std::pair<int,int> newPosition) {
	coords.clear();
	for (int i = 0; i < length; i++) {
		// Here the orientation refers to where the head of the ship points. So if the orientation is LEFT for example the rest of the ship expands to the right, etc.
		// TODO:This is pretty confusing and bad I should probably change this later
		switch (orientation) {
		case Orientation::LEFT:
			coords.push_back(newPosition + std::make_pair(0, i));
			break;
		case Orientation::UP:
			coords.push_back(newPosition + std::make_pair(i, 0));
			break;
		case Orientation::RIGHT:
			coords.push_back(newPosition + std::make_pair(0, -i));
			break;
		case Orientation::DOWN:
			coords.push_back(newPosition + std::make_pair(-i, 0));
			break;
		}
	}

	position.x = coords[0].second * Board::SQUARE_PIXEL_SIZE + 600;
	position.y = coords[0].first * Board::SQUARE_PIXEL_SIZE;
	
}


void Ship::rotate() {
	std::pair<std::pair<int, int>, std::pair<int, int>> rotationMatrix = { { 0 , 1}, { -1, 0 } };
	for (auto& coord : coords) {
		auto localCoord = vectorMatrixProduct2(coord, rotationMatrix);
	}
	orientation = static_cast<Orientation>((static_cast<int>(orientation) + 1) % 4);

	rotation -= 90 % 360;
	snapToPosition(coords[0]);

}

void Ship::draw(SpriteRenderer& renderer) {
	renderer.DrawSprite(this->sprite, this->position, this->size, this->rotation, this->color, glm::vec2(Board::SQUARE_PIXEL_SIZE / 2, Board::SQUARE_PIXEL_SIZE / 2), tex2);
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

	

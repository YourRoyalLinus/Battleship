#pragma once
#include <vector>
#include <utility>
#include <map>
#include <memory>
#include <SDL_image.h>
#include <string>


class Ship {
public:
	enum class Type {
		CARRIER,
		BATTLESHIP,
		CRUISER,
		SUBMARINE,
		DESTROYER,
		EMPTY
	};

	//TODO: this should be const and static but idk wtf I'm doing.
	std::map<Type, int> sizes = { {Type::CARRIER,5},{Type::BATTLESHIP,4}, {Type::CRUISER,3}, {Type::SUBMARINE,3}, {Type::DESTROYER,2}, {Type::EMPTY,0} };

	/*This is for moving the ship during setup*/
	enum class Direction {
		LEFT,
		RIGHT,
		UP,
		DOWN
	};

	Ship(Type type);
	Ship(): Ship(Type::EMPTY) {}

	/*Moved the ship around during setup*/
	void snapToPosition(std::pair<int,int> position);

	//Rotating needs to be more complicated but this is a hack for now. Doing this will update the coords when you next call snapToPosition.
	void rotate() { rotated = !rotated; }

	//TODO: Where does this go? Who knows
	void draw();

	Type type;
	std::vector<std::pair<int, int>> coords;

private:
	int length;
	bool rotated;
	bool placed;
	SDL_Texture* texture;
	static const std::string imgPath;
	
};


/* Helper function to move coordinates */
std::pair<int, int> operator+(const std::pair<int, int>& lhs, const std::pair<int, int>& rhs);




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

	enum class Orientation {
	    LEFT,
		UP,
		RIGHT,
		DOWN
	};

	//TODO: this should be const and static but idk wtf I'm doing.
	std::map<Type, int> sizes = { {Type::CARRIER,5},{Type::BATTLESHIP,4}, {Type::CRUISER,3}, {Type::SUBMARINE,3}, {Type::DESTROYER,2}, {Type::EMPTY,0} };
	std::map<Type, std::string> images = { {Type::CARRIER,"Textures\\Carrier.png"},{Type::BATTLESHIP,"Textures\\Battleship.png"}, {Type::CRUISER,"Textures\\Cruiser.png"}, {Type::SUBMARINE,"Textures\\Submarine2.png"}, {Type::DESTROYER,"Textures\\Destroyer2.png"}, {Type::EMPTY,""} };

	Ship(Type type);
	Ship(): Ship(Type::EMPTY) {}

	/*Moved the ship around during setup*/
	void snapToPosition(std::pair<int,int> position);

	//Rotating needs to be more complicated but this is a hack for now. Doing this will update the coords when you next call snapToPosition.
	void rotate();

	//TODO: Where does this go? Who knows
	void draw();

	bool sunk() { return hitsTaken >= length; }

	Type type;
	std::vector<std::pair<int, int>> coords;
	int hitsTaken;

private:
	int length;
	Orientation orientation;
//	std::shared_ptr<Texture> texture;
	std::string imgPath;
	std::vector<std::pair<int, int>> getCoordsRelativeToPivot(std::pair<int,int> pivot);
	int dotPoduct2(std::pair<int, int> vec1, std::pair<int, int> vec2);
	std::pair<int, int> vectorMatrixProduct2(std::pair<int, int> vec, std::pair<std::pair<int, int>, std::pair<int, int>> mat);
	
};


/* Helper function to move coordinates */
std::pair<int, int> operator+(const std::pair<int, int>& lhs, const std::pair<int, int>& rhs);




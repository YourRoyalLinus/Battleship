#pragma once
#include "Entity.h"
#include <glm.hpp>
#include "Game.h"
#include <map>
#include <string>
#include "ResourceManager.h"

class Marker : public Entity {

public:
	enum class Type {
		HIT,
		MISS,
		RADAR_HIT,
		RADAR_MISS
	};

	static std::map<Type, std::string> sprites;
	
	Marker(Type type, glm::vec2 pos) : Entity(pos, glm::vec2(Game::SQUARE_PIXEL_SIZE), ResourceManager::getTexture(sprites[type])) { /* ... */ }
};

std::map<Marker::Type, std::string> Marker::sprites = { {Type::HIT, "hit"}, { Type::MISS, "miss" }, { Type::RADAR_HIT, "radar_hit" }, { Type::RADAR_MISS, "radar_miss" } };


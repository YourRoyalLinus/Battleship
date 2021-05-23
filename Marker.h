#pragma once
#include <map>
#include <string>
#include "ResourceManager.h"
#include "Entity.h"
#include "Game.h"

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

std::map<Marker::Type, std::string> Marker::sprites = { {Type::HIT, "hit"}, { Type::MISS, "miss" }, { Type::RADAR_HIT, "circle" }, { Type::RADAR_MISS, "radar_miss" } };


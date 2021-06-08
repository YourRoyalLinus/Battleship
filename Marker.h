#pragma once
#include <map>
#include <string>
#include "ResourceManager.h"
#include "Entity.h"
#include "GameParams.h"

class Marker : public Entity {

	public:
		enum class Type {
			HIT,
			MISS,
			RADAR_HIT,
			RADAR_MISS
		};

		static std::map<Type, std::string> sprites;
	
	Marker(Type type, glm::vec2 pos) : Entity(pos, glm::vec2(GameParams::SQUARE_PIXEL_SIZE), ResourceManager::getTexture(sprites[type])) { /* ... */ }
};



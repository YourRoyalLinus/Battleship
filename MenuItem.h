#pragma once
#include "Entity.h"
class MenuItem : public Entity
{
public:
	enum class ItemType {
		SOLO,
		PVP,
		EASY,
		MEDIUM,
		HARD,
		BACK
	};
	//IDK if this is the right way to do this...
	MenuItem(ItemType type, glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec4 color = glm::vec4(1.0f)) : Entity(pos, size, sprite, color), type(type){ /*...*/ }

	ItemType type;
};

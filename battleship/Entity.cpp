#pragma once
#include "Entity.h"

//Default Entity is positioned at origin with unit size and no texture.
Entity::Entity() : position(0.0f, 0.0f), size(1.0f, 1.0f), rotation(0.0f), sprite(), color(1.0f,1.0f,1.0f,1.0f) { /*...*/ }

Entity::Entity(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec4 color):
position(pos), size(size), rotation(0.0f), sprite(sprite), color(color){ /*...*/ }

void Entity::draw(SpriteRenderer& renderer){
	renderer.DrawSprite(this->sprite, this->position, this->size, this->rotation, this->color, glm::vec2(), this->tex2);
}




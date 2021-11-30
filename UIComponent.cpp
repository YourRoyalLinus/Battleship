#include "UIComponent.h"
#include "Game.h"

UIComponent::UIComponent(Alignment alignment, Texture2D sprite, float lifetime, glm::vec4 color) :
	Entity(glm::vec2(0,0), glm::vec2(sprite.width, sprite.height), sprite, color), lifetime(lifetime), alignment(alignment)
{
	switch (alignment) {
	case Alignment::CENTER:
		position = glm::vec2(Game::SCREEN_WIDTH/2 - size.x/2, Game::SCREEN_HEIGHT/2 - size.y / 2); //Middle, Middle of screen
		break;
	
	case Alignment::TOP:
		position = glm::vec2(Game::SCREEN_WIDTH/2 - size.x/2, 10 + size.y/2); //Top, Middle of screen
		break;	
	case Alignment::TOP_RIGHT:
		position = glm::vec2((Game::SCREEN_WIDTH/2 + (600 - size.x)/2), 10 + size.y/2); //Top, Middle of screen
		break;
	case Alignment::TOP_LEFT:
		position = glm::vec2((Game::SCREEN_WIDTH/2 - (600 + size.x)/2), 10 + size.y/2); //Top, Middle of screen
		break;
	default:
		break;
	}
}

void UIComponent::update(float dt) {
	if (bouncing) {
		position.y += cos(dt * 5.0f) * 0.2f;
	}
	if (flashing) {
		color.a = (sin(dt * 3.0f) + 1.75f) / 2; //don't ask me
	}
	if (fadeOut) {
		color.a -= dt/1500.0f; //TRASH
	}
}

void UIComponent::scale(float amount) {
	this->size *= amount;
	realign();
}

void UIComponent::realign() {
	switch (alignment) {
	case Alignment::CENTER:
		position = glm::vec2(Game::SCREEN_WIDTH/2 - size.x/2, Game::SCREEN_HEIGHT/2 - size.y / 2); //Middle, Middle of screen
		break;
	
	case Alignment::TOP:
		position = glm::vec2(Game::SCREEN_WIDTH/2 - size.x/2, 10 + size.y/2); //Top, Middle of screen
		break;		
	case Alignment::TOP_RIGHT:
		position = glm::vec2((Game::SCREEN_WIDTH/2 + (600 - size.x)/2), 10 + size.y/2); //Top, Middle of screen
		break;
	case Alignment::TOP_LEFT:
		position = glm::vec2((Game::SCREEN_WIDTH/2 - (600 + size.x)/2), 10 + size.y/2); //Top, Middle of screen
		break;
	default:
		break;
	}
}


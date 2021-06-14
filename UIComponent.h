#pragma once
#include "Entity.h"
class UIComponent : public Entity
{
public:
	//Where on the screen the UICompenent starts
	enum class Alignment {
		NONE,
		CENTER,
		TOP,
		TOP_RIGHT,
		TOP_LEFT
	};
	UIComponent(Alignment alignment, Texture2D sprite, float lifetime, glm::vec4 color = glm::vec4(1.0f));
	void setBouncing(bool bouncing) { this->bouncing = bouncing; }
	void setFlashing(bool flashing) { this->flashing = flashing; }
	void setFadeOut(bool fading) { this->fadeOut = fading; }
	void scale(float amount);
	void update(float dt);
private:
	Alignment alignment;
	void realign();
	float lifetime;
	bool bouncing = false;
	bool flashing = false;
	bool fadeOut = false;
};


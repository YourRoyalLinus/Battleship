#pragma once
#include <glm.hpp>
#include "Texture2D.h"
#include "SpriteRenderer.h"

class Entity
{
    public:    

        glm::vec2 position, size;
        glm::vec4 color;
        float rotation;

        Texture2D sprite;
        Texture2D tex2;

        Entity();
        Entity(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec4 color = glm::vec4(1.0f));
    
        //Draw sprite
        virtual void draw(SpriteRenderer& renderer);
        void setTex2(Texture2D& texture) { this->tex2 = texture; }

        virtual ~Entity() = default;
};


#pragma once
#include <limits>
#include "Texture2D.h"
#include "Shader.h"

class SpriteRenderer
{
	public:
        SpriteRenderer(Shader &shader);
        ~SpriteRenderer();

        void DrawSprite(Texture2D& texture, glm::vec2 position,
            glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f,
            glm::vec4 color = glm::vec4(1.0f), glm::vec2 pointOfRotation = glm::vec2(), Texture2D& normalMap = Texture2D());
           
    private:
        Shader shader; 
        unsigned int quadVAO;

        void initRenderData();
};


#pragma once
#include "Texture2D.h"
#include "Shader.h"
#include <glm.hpp>
#include <limits>

class SpriteRenderer
{
	public:
        SpriteRenderer(Shader &shader);
        ~SpriteRenderer();

        void DrawSprite(Texture2D &texture, glm::vec2 position, 
            glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, 
            glm::vec4 color = glm::vec4(1.0f), glm::vec2 pointOfRotation = glm::vec2()); //TODO: <- this causes a bug if the caller wants to rotate around (0,0)

    private:
        Shader shader; 
        unsigned int quadVAO;

        void initRenderData();
};

#pragma once
#include "Shader.h"
#include "Texture2D.h"
#include "Entity.h"
#include <glm.hpp>
#include <vector>

struct Particle {
    glm::vec2 position, velocity;
    glm::vec4 color;
    float     life;

    Particle()
        : position(0.0f), velocity(0.0f), color(1.0f), life(0.0f) { }
};

//TODO: Right now Particle's are not like other "Entities" in our game because unlike other entities, they have a velocity and a lifetime.
//Because of this they don't inheret from Entity right now but if we change entity in the future they probably should?
class ParticleEmitter
{
public:
    ParticleEmitter(Shader shader, Texture2D texture, unsigned int amount, glm::vec2 square, glm::vec4 startColor, glm::vec4 endColor, unsigned int glBlendSrc = GL_SRC_ALPHA, unsigned int gLBlendDst = GL_ONE);

    void update(float dt, unsigned int numNewParticles, glm::vec2 offset = glm::vec2());

    void draw();

    glm::vec2 emmiterSquare;


private:
    std::vector<Particle> particles;
    unsigned int amountOfParticles;
    glm::vec4 startColor, endColor;
   
    GLuint VAO;
	Shader shader;
	Texture2D texture;
    glm::vec2 emmiterPosition;

    unsigned int glBlendSrc, glBlendDst;

    void init();

    unsigned int firstUnusedParticle();
    void respawnParticle(Particle& particle, glm::vec2 offset = glm::vec2());
    glm::vec4 colorLerp(float dt);
    float lerp(float start, float end, float t);
    float clamp(float val, float min, float max);


};


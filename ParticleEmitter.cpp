#include "ParticleEmitter.h"
#include <algorithm>
#include <cmath>
#include <random>
#include <chrono>
#include "GameParams.h"

ParticleEmitter::ParticleEmitter(Shader shader, Texture2D texture, unsigned int amount, glm::vec2 square, glm::vec4 startColor, glm::vec4 endColor, unsigned int glBlendSrc, unsigned int glBlendDst) :
	shader(shader), texture(texture), amountOfParticles(amount), emmiterSquare(square), emmiterPosition(glm::vec2(square.y * GameParams::SQUARE_PIXEL_SIZE + 600, square.x * GameParams::SQUARE_PIXEL_SIZE)), 
		startColor(startColor), endColor(endColor), glBlendSrc(glBlendSrc), glBlendDst(glBlendDst){
	this->init();
}

void ParticleEmitter::update(float dt, unsigned int numNewParticles, glm::vec2 offset) {

	//add new particles
	for (unsigned int i = 0; i < numNewParticles; i++) {
		int unusedParticleIndex = this->firstUnusedParticle();
		this->respawnParticle(this->particles[unusedParticleIndex], offset);
	}

	//update all particle positions, lifetimes and colors
	for (unsigned int i = 0; i < this->amountOfParticles; i++) {
		Particle& particle = this->particles[i];
		particle.life -= dt;
		if (particle.life > 0.0f) {
			//particle is alive!
			particle.position.y -= dt * (10 + rand() % 50);
			particle.color = colorLerp(1.0 - particle.life);
		}
	}
}

void ParticleEmitter::draw() {
	//ablending
	glBlendFunc(this->glBlendSrc, this->glBlendDst);
	this->shader.use();
	for (auto particle : this->particles) {
		if (particle.life > 0.0f) {
			//update shader uniform for position & color
			this->shader.setVec2("offset", particle.position);
			this->shader.setVec4("color", particle.color);
			//draw it
			this->texture.bind();
			glBindVertexArray(this->VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
			//std::cout << "Drew particle at:" << particle.position.x << " " << particle.position.y << std::endl;

		}
	}
	//reset openGL's blend function.
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleEmitter::init() {
	unsigned int VBO;
	float particle_quad[] = {
		//vertices  //tex coords
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(this->VAO);
	// fill buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
	// set attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glBindVertexArray(0);

	// create this->amount default particle instances
	for (unsigned int i = 0; i < this->amountOfParticles; ++i)
		this->particles.push_back(Particle());
}

//cached index of last dead particle for quicker searches.
unsigned int lastDeadParticle = 0;
unsigned int ParticleEmitter::firstUnusedParticle() {
	
	//search from the last dead particle. This should be fast!
	for (unsigned int i = lastDeadParticle; i < this->amountOfParticles; i++) {
		if (this->particles[i].life < 0.0f) {
			lastDeadParticle = i;
			return lastDeadParticle;
		}
	}

	//otherwise do linear search through whole vector
	for (unsigned int i = 0; i < this->amountOfParticles; i++) {
		if (this->particles[i].life < 0.0f) {
			lastDeadParticle = i;
			return lastDeadParticle;
		}
	}

	//if all particles are still alive, overwrite last one, This means the lifetime of your particles is too damn high!
	lastDeadParticle = 0;
	return lastDeadParticle;

}

void ParticleEmitter::respawnParticle(Particle& particle, glm::vec2 offset) {
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	//TODO add more options for different statistical distributions? Idk anything about stats
	std::default_random_engine generator(seed);
	std::normal_distribution<double> distribution(0, 8);
	float randomX = distribution(generator);
	float randomY = distribution(generator);
	if (randomY > 5) {
		randomY = 5;
	}
	particle.position = glm::vec2(emmiterPosition.x + randomX, emmiterPosition.y + randomY) + offset;
	particle.color = this->startColor;
	particle.life = 1.0f;
}

glm::vec4 ParticleEmitter::colorLerp(float t) {
	//TODO: There is almost certainly a library function do this. If not a more generic way to do this but for now:
	glm::vec4 result;
	result.r = lerp(startColor.r, endColor.r, t);
	result.g = lerp(startColor.g, endColor.g, t);
	result.b = lerp(startColor.b, endColor.b, t);
	result.a = lerp(startColor.a, endColor.a, t);
	return result;

}

float ParticleEmitter::lerp(float start, float end, float t) {
	//TODO: I'm going to want to move this out soon!
	float result = (1 - t)*start + (end*t);
	return result;
}

float ParticleEmitter::clamp(float val, float min, float max) {
	if (val > max) return max;
	else if (val < min) return min;
	else return val;
}



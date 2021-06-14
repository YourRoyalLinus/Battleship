#pragma once
#include <chrono>
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include <SDL.h>
#include "Game.h"
#include "ResourceManager.h"
#include <stb_image.h>
#include "Texture2D.h"
#include "MenuState.h"
#include "MenuItem.h"
#include "SinglePlayerSetup.h"
#include "Marker.h"
#include <thread>

Game::Game(){
}

Game::~Game() {
	delete net;
	delete player;
}

void Game::init() {
	//shaders for sprites
	ResourceManager::loadShader("basic_sprite.vert", "water.frag", "water");
	ResourceManager::loadShader("basic_sprite.vert", "basic_sprite.frag", "basic_sprite");
	ResourceManager::loadShader("basic_sprite.vert", "radar.frag", "radar2");
	ResourceManager::loadShader("particle.vert", "particle.frag", "particle");
	ResourceManager::loadShader("basic_sprite.vert", "grid.frag", "grid");
	ResourceManager::loadShader("basic_sprite.vert", "ship.frag", "ship");

	//postprocessing shader
	ResourceManager::loadShader("postprocessing.vert", "postprocessing.frag", "postprocessing");

	//Create Orthographic Projection Matrix
	glm::mat4 projection = glm::ortho<GLfloat>(0.0f, static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT), 0.0f, -1.0f, 1.0f);
	//Configure shaders
	ResourceManager::getShader("water").use().setInt("image", 0);
	ResourceManager::getShader("water").setInt("waveMap", 1);
	ResourceManager::getShader("water").setMat4("projection", projection);
	ResourceManager::getShader("basic_sprite").use().setInt("image", 0);
	ResourceManager::getShader("basic_sprite").setMat4("projection", projection);
	ResourceManager::getShader("particle").use().setInt("image", 0);
	ResourceManager::getShader("particle").setMat4("projection", projection);
	ResourceManager::getShader("grid").use().setInt("image", 0);
	ResourceManager::getShader("grid").setMat4("projection", projection);
	ResourceManager::getShader("ship").use().setInt("image", 0);
	ResourceManager::getShader("ship").setInt("sunGradient", 1);
	ResourceManager::getShader("ship").setMat4("projection", projection);

	//radar test
	ResourceManager::getShader("radar2").use().setInt("image", 0);
	ResourceManager::getShader("radar2").setInt("pings", 1);
	ResourceManager::getShader("radar2").setMat4("projection", projection);
	ResourceManager::getShader("radar2").setFloat("radius", 0.62f);
	ResourceManager::getShader("radar2").setFloat("glowStrength", 0.2f);
	ResourceManager::getShader("radar2").setVec3("color", glm::vec3(0.0f, 0.6431372549019608f, 0.09019607843137255f)); //green
	ResourceManager::getShader("radar2").setFloat("fillStrength", 0.3f);
	ResourceManager::getShader("radar2").setVec2("resolution", glm::vec2(600.0f, 600.0f));
	ResourceManager::getShader("radar2").setFloat("turn", 0.0f);


	//Load Textures
	// --Menu stuff--
	ResourceManager::loadTexture("Textures\\MenuTitle.png", GL_RGBA, GL_RGBA, "title");
	ResourceManager::loadTexture("Textures\\Solo.png", GL_RGBA, GL_RGBA, "solo");
	ResourceManager::loadTexture("Textures\\PVP.png", GL_RGBA, GL_RGBA, "pvp");
	ResourceManager::loadTexture("Textures\\Easy.png", GL_RGBA, GL_RGBA, "easy");
	ResourceManager::loadTexture("Textures\\Medium.png", GL_RGBA, GL_RGBA, "medium");
	ResourceManager::loadTexture("Textures\\Hard.png", GL_RGBA, GL_RGBA, "hard");
	ResourceManager::loadTexture("Textures\\Back.png", GL_RGBA, GL_RGBA, "back");
	ResourceManager::loadTexture("Textures\\Waiting.png", GL_RGBA, GL_RGBA, "waiting");
	ResourceManager::loadTexture("Textures\\Searching.png", GL_RGBA, GL_RGBA, "searching");
	ResourceManager::loadTexture("Textures\\Cursor.png", GL_RGBA, GL_RGBA, "cursor");
	ResourceManager::loadTexture("Textures\\PlaceShips.png", GL_RGBA, GL_RGBA, "placeShips");
	ResourceManager::loadTexture("Textures\\YourTurn.png", GL_RGBA, GL_RGBA, "yourTurn");
	ResourceManager::loadTexture("Textures\\OppTurn.png", GL_RGBA, GL_RGBA, "oppTurn");

	// --grids--
	ResourceManager::loadTexture("Textures\\WaterGrid2.png", GL_RGBA, GL_RGBA, "grid");
	ResourceManager::loadTexture("Textures\\GuessBoard2.png", GL_RGBA, GL_RGBA, "radar");

	ResourceManager::loadTexture("Textures\\pings.png", GL_RGB, GL_RGB, "pings");
	ResourceManager::loadTexture("Textures\\waterGradient.png", GL_RGB, GL_RGB, "water");
	ResourceManager::loadTexture("Textures\\waveTexture.png", GL_RED, GL_RED, "waveMap");

	// --ships--
	ResourceManager::loadTexture("Textures\\Destroyer2.png", GL_RGBA, GL_RGBA, "destroyer");
	ResourceManager::loadTexture("Textures\\DestroyerNormalMap.png", GL_RGBA, GL_RGBA, "destroyerNormalMap");
	ResourceManager::loadTexture("Textures\\Submarine2.png", GL_RGBA, GL_RGBA, "submarine");
	ResourceManager::loadTexture("Textures\\Carrier.png", GL_RGBA, GL_RGBA, "carrier");
	ResourceManager::loadTexture("Textures\\Cruiser.png", GL_RGBA, GL_RGBA, "cruiser");
	ResourceManager::loadTexture("Textures\\Battleship.png", GL_RGBA, GL_RGBA, "battleship");

	ResourceManager::loadTexture("Textures\\sunGradient.png", GL_RGB, GL_RGB, "sunGradient");
	// --hitMarkers--
	ResourceManager::loadTexture("Textures\\BattleShip_Radar_Miss.png", GL_RGBA, GL_RGBA, "radar_miss");
	ResourceManager::loadTexture("Textures\\BattleShip_Miss.png", GL_RGBA, GL_RGBA, "miss");
	ResourceManager::loadTexture("Textures\\BattleShip_Hit.png", GL_RGBA, GL_RGBA, "hit");
	//particles
	ResourceManager::loadTexture("Textures\\fireparticle.png", GL_RGBA, GL_RGBA, "circle");


	
	effects = new PostProcessor(ResourceManager::getShader("postprocessing"), SCREEN_WIDTH, SCREEN_HEIGHT);

	waterRenderer = new SpriteRenderer(ResourceManager::getShader("water"));
	gridRenderer = new SpriteRenderer(ResourceManager::getShader("grid"));
    spriteRenderer = new SpriteRenderer(ResourceManager::getShader("basic_sprite"));
	radarBoardRenderer = new SpriteRenderer(ResourceManager::getShader("radar2"));
	shipRenderer = new SpriteRenderer(ResourceManager::getShader("ship"));

	grid = new Entity(glm::vec2(600.0f, 0.0f), glm::vec2(600.0f, 600.0f), ResourceManager::getTexture("grid"));
	menuWater = new Entity(glm::vec2(0.0f, 0.0f), glm::vec2(1200.0f, 600.0f), ResourceManager::getTexture("water"));
	menuWater->tex2 = ResourceManager::getTexture("waveMap");

	state = new MenuState(*this);
}

void Game::update(float dt) {
	state->update();

	
	for (auto fireEmitter = fireEmitters.begin(); fireEmitter != fireEmitters.end(); fireEmitter++) {
		fireEmitter->update(dt, 8, glm::vec2(35.0f));
	}
	for (auto& smokeEmitter : smokeEmitters) {
		smokeEmitter.update(dt, 3, glm::vec2(35.0f, 15.0f));
	}

	//update postprocessing effects
	if (effects->shake && shakeTime > 0.0) {
		shakeTime -= dt;
		if (shakeTime <= 0.0) {
			effects->shake = false;
		}
	}

}


void Game::render(float dt) {
	state->render();	
}


void Game::spawnFire(std::pair<int,int> square) {
	glm::vec2 squarePos = glm::vec2(square.first, square.second);
	ParticleEmitter fireEmitter(ResourceManager::getShader("particle"), ResourceManager::getTexture("circle"), 1000, squarePos, glm::vec4(0.8, 0.2, 0.0, .3), glm::vec4(1.0, 0.0, 0.0, 0.0));
	ParticleEmitter smokeEmitter(ResourceManager::getShader("particle"), ResourceManager::getTexture("circle"), 300, squarePos, glm::vec4(0.2, 0.2, 0.2, 0.05), glm::vec4(0.0, 0.0, 0.0, 0.0),
		GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	fireEmitters.push_back(fireEmitter);
	smokeEmitters.push_back(smokeEmitter);
}

void Game::removeUnderwaterFire() {
	//Check if any of the ships where the fire/smoke emmitters are place have been sunk (ie. those squares are no longer accupied & remove them if they have)
	//This logic is pretty gnarly because removing an iterator invalidates the iterator.
	auto fireEmitter = fireEmitters.begin();
	auto smokeEmitter = smokeEmitters.begin();
	while (fireEmitter != fireEmitters.end()) 
	{
		std::pair<int, int> emmiterPos = { fireEmitter->emmiterSquare.x, fireEmitter->emmiterSquare.y};
		if (!player->board->squareOccupied(emmiterPos)) {
			fireEmitter = fireEmitters.erase(fireEmitter);
			smokeEmitter = smokeEmitters.erase(smokeEmitter);
			//std::cout << "Emmitter at " << emmiterPos.first << " " << emmiterPos.second << " should be deactivated" << std::endl;
		}
		else {
			smokeEmitter++;
			fireEmitter++;
		}
	}
}

void Game::renderRadarPings() {
	//render radar information to offscreen buffer based of hit's and misses from player.
	unsigned int FBO;
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	//attach texture to our fbo
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ResourceManager::getTexture("pings").ID, 0);
	//check if FBO is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::RADAR PINGS: Failed to initialize FBO" << std::endl;
    
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	//now actually render to the texture.
	for (auto square : opponent->board->guessedSquares) {
		if (square.occupied) {
			Marker hit(Marker::Type::RADAR_HIT, glm::vec2(square.col * SQUARE_PIXEL_SIZE, square.row * SQUARE_PIXEL_SIZE));
			hit.draw(*spriteRenderer);
		}
		else {
			Marker miss(Marker::Type::RADAR_MISS, glm::vec2(square.col * SQUARE_PIXEL_SIZE, square.row * SQUARE_PIXEL_SIZE));
			miss.draw(*spriteRenderer);
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &FBO);
}

float Game::mticks()
{
    typedef std::chrono::high_resolution_clock clock;
    typedef std::chrono::duration<float, std::milli> duration;

    static clock::time_point start = clock::now();
    duration elapsed = clock::now() - start;
    return elapsed.count() / 1000;
}

void Game::onNotify(Event* event) {
	switch (event->eventType) {
	case Event::Type::PLAYER_HIT: {
		if (activePlayer == opponent) {
			//this if check is technically redunant right now!
			PlayerHit* hitEvent = static_cast<PlayerHit*>(event); //THIS IS TERRIBLE. NOT SURE THE CORRECT WAY TO DO THIS
			spawnFire(hitEvent->coord);
			shakeScreen();
		}
		break;
	}
	case Event::Type::SHIP_SANK: { break; }
	default: { break; }
	}
	delete event;
}
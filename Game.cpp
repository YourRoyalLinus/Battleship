#include "Game.h"
#include "ResourceManager.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <SDL.h>
#include <chrono>
#include "Marker.h"
#include <stb_image.h>
#include "Texture2D.h"

Game::Game() : state(GameState::SETUP), human(nullptr), computer(nullptr), playerBoard(nullptr), radarBoard(nullptr), humanTurn(true),
			   waterRenderer(nullptr), spriteRenderer(nullptr), mousePosX(0), mousePosY(0), leftClick(false), rightClick(false), shipToPlace(nullptr) { /*...*/ }

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


	//Load Textures

	// --grids--
	ResourceManager::loadTexture("Textures\\WaterGrid2.png", GL_RGBA, GL_RGBA, "grid");
	ResourceManager::loadTexture("Textures\\GuessBoard2.png", GL_RGBA, GL_RGBA, "radar");

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
	//this texture isn't cooperating but I want to make something better evenetually anyway so Idc that much.
	//ResourceManager::loadTexture("Textures\\BattleShip_Radar_Hit.png", false, "radar_hit");
	ResourceManager::loadTexture("Textures\\BattleShip_Miss.png", GL_RGBA, GL_RGBA, "miss");
	ResourceManager::loadTexture("Textures\\BattleShip_Hit.png", GL_RGBA, GL_RGBA, "hit");
	//particles
	ResourceManager::loadTexture("Textures\\fireparticle.png", GL_RGBA, GL_RGBA, "circle");
	ResourceManager::loadTexture("Textures\\pings.png", GL_RGB, GL_RGB, "pings");
	
	effects = new PostProcessor(ResourceManager::getShader("postprocessing"), SCREEN_WIDTH, SCREEN_HEIGHT);

	waterRenderer = new SpriteRenderer(ResourceManager::getShader("water"));
	gridRenderer = new SpriteRenderer(ResourceManager::getShader("grid"));
    spriteRenderer = new SpriteRenderer(ResourceManager::getShader("basic_sprite"));
	radarBoardRenderer = new SpriteRenderer(ResourceManager::getShader("radar2"));
	shipRenderer = new SpriteRenderer(ResourceManager::getShader("ship"));

	playerBoard = new Board(Board::Type::PLAYER);
	radarBoard = new Board(Board::Type::RADER);

	grid = new Entity(glm::vec2(600.0, 0.0), glm::vec2(600.0, 600.0), ResourceManager::getTexture("grid"));

	human = new Player();
	computer = new Player();

}

void Game::handleInput() {
	SDL_Event event;
	if (SDL_PollEvent(&event) != NULL) {
		if (event.type == SDL_QUIT) {
			exit(0);
		}
		SDL_GetMouseState(&mousePosX, &mousePosY);
		if (event.type == SDL_MOUSEBUTTONDOWN) {
			if (event.button.button == SDL_BUTTON_LEFT)
				leftClick = true;
			else if (event.button.button == SDL_BUTTON_RIGHT)
				rightClick = true;
		}


	}
}

void Game::update(float dt) {
	//Hotload shaders
	updateShaders();
	if (state == GameState::SETUP) {

		if (humanTurn) {

			if (human->ships.empty()) {
				//The player has placed all the ships
				humanTurn = false;
				return;
			}


			Ship& currentShipRef = human->ships.back();

			/* Clamp position to player board grid square */
			int mouseXSquare = (mousePosX - SCREEN_WIDTH/2) / Board::SQUARE_PIXEL_SIZE;
			int mouseYSquare = (mousePosY) / Board::SQUARE_PIXEL_SIZE;


			currentShipRef.snapToPosition({ mouseYSquare, mouseXSquare }); //Ypos = row, Xpos = col.


			if (leftClick) {
				if (playerBoard->placeShip(currentShipRef, currentShipRef.coords)) {
					human->ships.pop_back();
					playerBoard->print();
					std::cout << "\n\n" << std::endl;
				}
				else { printf("You can't place a ship there!\n"); }
				leftClick = false; // "Consumed" left-click event,
			}
			else if (rightClick) {
				currentShipRef.rotate(); //TODO: Rotations are broken right now
				rightClick = false;
			}



		}

		else{
			//Computer's turn
			//Computer places all it's ships
			while (!computer->ships.empty()) {
				Ship& computerShipRef = computer->ships.back();

				int row = rand() % 8;
				int col = rand() % 8;
				
				computerShipRef.snapToPosition({ row, col });

				if (radarBoard->placeShip(computerShipRef, computerShipRef.coords))
					computer->ships.pop_back();
			
			}
			//Computer is done placing all it's ships move on to next state.
			state = GameState::PLAYING;
			humanTurn = true;

		//	radarBoard->print();

		}
	}
	else if(state == GameState::PLAYING){
		if (humanTurn) {
			if (leftClick) {
				/* Clamp position to player board grid square */
				int mouseXSquare = (mousePosX) / Board::SQUARE_PIXEL_SIZE;
				int mouseYSquare = (mousePosY) / Board::SQUARE_PIXEL_SIZE;
				if (radarBoard->guess({ mouseYSquare, mouseXSquare }))
					std::cout << "Player hit!" << std::endl;
				humanTurn = false;
				leftClick = false;
			}
		}
		else {
			int row = rand() % 8;
			int col = rand() % 8;
			if (playerBoard->guess({ row, col })) {
				//Computer hit a player's ship. Draw fire effects and configure screen shake post processing effect.
				glm::vec2 squarePostion = glm::vec2(row, col);
				ParticleEmitter fireEmitter(ResourceManager::getShader("particle"), ResourceManager::getTexture("circle"), 1000, squarePostion, glm::vec4(0.8, 0.2, 0.0, .3), glm::vec4(1.0, 0.0, 0.0, 0.0));
				ParticleEmitter smokeEmitter(ResourceManager::getShader("particle"), ResourceManager::getTexture("circle"), 100, squarePostion, glm::vec4(0.2,0.2,0.2,0.05), glm::vec4(0.0,0.0,0.0,0.0),
				GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				fireEmitters.push_back(fireEmitter);
				smokeEmitters.push_back(smokeEmitter);

				shakeTime = 0.2f;
				effects->shake = true;

				std::cout << "Computer hit!" << std::endl;
			}
			humanTurn = true;
		}

		

		if (playerBoard->activeShips.empty()) {
			std::cout << "Computer Won!" << std::endl;
			state = GameState::OVER;
		}
		else if (radarBoard->activeShips.empty()) {
			std::cout << "Player Won!" << std::endl;
			state = GameState::OVER;
		}

	}
	else if (state == GameState::OVER) {
		exit(0);
	}

	//Check if any of the ships where the fire/smoke emmitters are place have been sunk (ie. those squares are no longer accupied & remove them if they have)
	//This logic is pretty gnarly because removing an iterator invalidates the iterator.

	auto fireEmitter = fireEmitters.begin();
	auto smokeEmitter = smokeEmitters.begin();
	while (fireEmitter != fireEmitters.end()) 
	{
		std::pair<int, int> emmiterPos = { fireEmitter->emmiterSquare.x, fireEmitter->emmiterSquare.y};
		if (!playerBoard->squareOccupied(emmiterPos)) {
			fireEmitter = fireEmitters.erase(fireEmitter);
			smokeEmitter = smokeEmitters.erase(smokeEmitter);
			//std::cout << "Emmitter at " << emmiterPos.first << " " << emmiterPos.second << " should be deactivated" << std::endl;
		}
		else {
			smokeEmitter++;
			fireEmitter++;
		}
	}
	for (auto fireEmitter = fireEmitters.begin(); fireEmitter != fireEmitters.end(); fireEmitter++) {
		fireEmitter->update(dt, 8, glm::vec2(35.0f));
	}
	for (auto& smokeEmitter : smokeEmitters) {
		smokeEmitter.update(dt, 1, glm::vec2(35.0f, 15.0f));
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
	renderRadarPings();

	//update uniforms
	ResourceManager::getShader("grid").use().setFloat("iTime", mticks());
	ResourceManager::getShader("water").use().setFloat("iTime", mticks());

	//testing new radar effect.
	//TODO: these don't need to be done with uniforms right now.
	ResourceManager::getShader("radar2").use().setFloat("time", mticks());
	ResourceManager::getShader("radar2").use().setFloat("radius", 0.62f);
	ResourceManager::getShader("radar2").use().setFloat("glowStrength", 0.2f);
	ResourceManager::getShader("radar2").use().setVec3("color", glm::vec3(0.0f, 0.6431372549019608f, 0.09019607843137255f)); //green
	ResourceManager::getShader("radar2").use().setFloat("fillStrength", 0.3f);
	ResourceManager::getShader("radar2").use().setVec2("resolution", glm::vec2(600.0f, 600.0f));




	//Render to off-screen buffer for postprocessing effects
	effects->beginRender();

	radarBoard->draw(*radarBoardRenderer);
	playerBoard->draw(*waterRenderer);
	glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
	grid->draw(*gridRenderer);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	

	for(auto square : playerBoard->guessedSquares){
		if (!square.occupied) {
			Marker miss(Marker::Type::MISS, glm::vec2(600 + square.col * SQUARE_PIXEL_SIZE, square.row * SQUARE_PIXEL_SIZE));
			miss.draw(*spriteRenderer);
		}
	}

	//draw placed ships on player's board
	for (auto& ship : playerBoard->activeShips) {
		ship.draw(*shipRenderer);
	}

	for (auto& fireEmitter : fireEmitters) {
		fireEmitter.draw();
	}
	for (auto& smokeEmitter : smokeEmitters) {
		smokeEmitter.draw();
	}

	

	//draw placing ship if you are in setup
	if (state == GameState::SETUP) {
		if (!human->ships.empty())
			shipToPlace = &(human->ships.back());

		//If there is a ship currently to place and it's current inside the player board part of the screen, draw it.
		if (shipToPlace != nullptr && shipToPlace->position.x >= 600.0f) {
			shipToPlace->draw(*shipRenderer);		
		}
	}	


	//after redering whole scene to off-screen buffer, apply postprocessing affects and blit to screen.
	effects->endRender();
	effects->render(shakeTime);
	
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
	for (auto square : radarBoard->guessedSquares) {
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
}

float Game::mticks()
{
    typedef std::chrono::high_resolution_clock clock;
    typedef std::chrono::duration<float, std::milli> duration;

    static clock::time_point start = clock::now();
    duration elapsed = clock::now() - start;
    return elapsed.count() / 1000;
}

void Game::updateShaders() {
	ResourceManager::hotReload();
	delete waterRenderer;
	waterRenderer = nullptr;
	delete radarBoardRenderer;
	radarBoardRenderer = nullptr;
	delete spriteRenderer;
	spriteRenderer = nullptr;
	
	glm::mat4 projection = glm::ortho<GLfloat>(0.0f, static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT), 0.0f, -1.0f, 1.0f);

	//Configure shaders
	ResourceManager::getShader("water").use().setInt("image", 0);
	ResourceManager::getShader("water").setMat4("projection", projection);
	ResourceManager::getShader("basic_sprite").use().setInt("image", 0);
	ResourceManager::getShader("basic_sprite").setMat4("projection", projection);
	ResourceManager::getShader("radar2").use().setInt("image", 0);
	ResourceManager::getShader("radar2").setMat4("projection", projection);



	waterRenderer = new SpriteRenderer(ResourceManager::getShader("water"));
    spriteRenderer = new SpriteRenderer(ResourceManager::getShader("basic_sprite"));
	radarBoardRenderer = new SpriteRenderer(ResourceManager::getShader("radar2"));

}


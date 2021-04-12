#include "Game.h"
#include "ResourceManager.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <SDL.h>
#include <chrono>
#include "Marker.h"

Game::Game() : state(GameState::SETUP), human(nullptr), computer(nullptr), playerBoard(nullptr), radarBoard(nullptr), humanTurn(true),
			   playerBoardRenderer(nullptr), spriteRenderer(nullptr), mousePosX(0), mousePosY(0), leftClick(false), rightClick(false), shipToPlace(nullptr) { /*...*/ }

void Game::init() {
	ResourceManager::loadShader("basic_sprite.vert", "water_grid.frag", "water_grid");
	ResourceManager::loadShader("basic_sprite.vert", "basic_sprite.frag", "basic_sprite");

	//Testing radar effect 
	ResourceManager::loadShader("basic_sprite.vert", "radar.frag", "radar2");

	//Create Orthographic Projection Matrix
	glm::mat4 projection = glm::ortho<GLfloat>(0.0f, static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT), 0.0f, -1.0f, 1.0f);
	//Configure shaders
	ResourceManager::getShader("water_grid").use().setInt("image", 0);
	ResourceManager::getShader("water_grid").setMat4("projection", projection);
	ResourceManager::getShader("basic_sprite").use().setInt("image", 0);
	ResourceManager::getShader("basic_sprite").setMat4("projection", projection);

	//radar test
	ResourceManager::getShader("radar2").use().setInt("image", 0);
	ResourceManager::getShader("radar2").setMat4("projection", projection);


	//Load Textures

	// --grids--
	ResourceManager::loadTexture("Textures\\BattleshipGrid.png", true, "grid");
	ResourceManager::loadTexture("Textures\\GuessBoard2.png", true, "radar");
	// --ships--
	ResourceManager::loadTexture("Textures\\Destroyer2.png", true, "destroyer");
	ResourceManager::loadTexture("Textures\\Submarine2.png", true, "submarine");
	ResourceManager::loadTexture("Textures\\Carrier.png", true, "carrier");
	ResourceManager::loadTexture("Textures\\Cruiser.png", true, "cruiser");
	ResourceManager::loadTexture("Textures\\Battleship.png", true, "battleship");
	// --hitMarkers--
	ResourceManager::loadTexture("Textures\\BattleShip_Radar_Miss.png", true, "radar_miss");
	//this texture isn't cooperating but I want to make something better evenetually anyway so Idc that much.
	//ResourceManager::loadTexture("Textures\\BattleShip_Radar_Hit.png", false, "radar_hit");
	ResourceManager::loadTexture("Textures\\BattleShip_Miss.png", true, "miss");
	ResourceManager::loadTexture("Textures\\BattleShip_Hit.png", true, "hit");



	playerBoardRenderer = new SpriteRenderer(ResourceManager::getShader("water_grid"));
    spriteRenderer = new SpriteRenderer(ResourceManager::getShader("basic_sprite"));
	radarBoardRenderer = new SpriteRenderer(ResourceManager::getShader("radar2"));

	playerBoard = new Board(Board::Type::PLAYER);
	radarBoard = new Board(Board::Type::RADER);

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

void Game::update() {
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
				std::cout << "Computer hit!" << std::endl;
			}
			humanTurn = true;
		}

		if (playerBoard->activeShips.empty()) {
			std::cout << "Computer Won!" << std::endl;
			state = GameState::OVER;
		}
		else if (radarBoard->activeShips.empty()) {
			std::cout << "Plyaer Won!" << std::endl;
			state = GameState::OVER;
		}

	}
	else if (state == GameState::OVER) {
		exit(0);
	}

}

void Game::render() {
	//update uniforms
	ResourceManager::getShader("water_grid").use().setFloat("iTime", mticks());
	//draw boards
	

	//testing new radar effect.
	//TODO: these don't need to be done with uniforms right now.
	ResourceManager::getShader("radar2").use().setFloat("time", mticks());
	ResourceManager::getShader("radar2").use().setFloat("radius", 0.62f);
	ResourceManager::getShader("radar2").use().setFloat("glowStrength", 0.3f);
	ResourceManager::getShader("radar2").use().setVec3("color", glm::vec3(0.0f, 0.6431372549019608f, 0.09019607843137255f)); //green
	ResourceManager::getShader("radar2").use().setFloat("fillStrength", 0.3f);
	ResourceManager::getShader("radar2").use().setVec2("resolution", glm::vec2(600.0f, 600.0f));


	radarBoard->draw(*radarBoardRenderer);
	playerBoard->draw(*playerBoardRenderer);

	//draw placed ships on player's board
	for (auto& ship : playerBoard->activeShips) {
		ship.draw(*spriteRenderer);
	}

	//draw placing ship if you are in setup
	if (state == GameState::SETUP) {
		if (!human->ships.empty())
			shipToPlace = &(human->ships.back());

		//If there is a ship currently to place and it's current inside the player board part of the screen, draw it.
		if (shipToPlace != nullptr && shipToPlace->position.x >= 600.0f)
			shipToPlace->draw(*spriteRenderer);
	}

	//draw hit markers on board
	//TODO: This is the worst code ever wtf am I doing
	for (auto square : playerBoard->guessedSquares) {
		if (square.occupied) {
			Marker hit(Marker::Type::HIT, glm::vec2(square.col * SQUARE_PIXEL_SIZE + SCREEN_WIDTH/2 , square.row * SQUARE_PIXEL_SIZE));
			hit.draw(*spriteRenderer);
		}
		else {
			Marker miss(Marker::Type::MISS, glm::vec2(square.col * SQUARE_PIXEL_SIZE + SCREEN_WIDTH/2 , square.row * SQUARE_PIXEL_SIZE));
			miss.draw(*spriteRenderer);
		}
	}

	for (auto square : radarBoard->guessedSquares) {
		if (square.occupied) {
			Marker hit(Marker::Type::HIT, glm::vec2(square.col * SQUARE_PIXEL_SIZE, square.row * SQUARE_PIXEL_SIZE));
			hit.draw(*spriteRenderer);
		}
		else {
			Marker miss(Marker::Type::RADAR_MISS, glm::vec2(square.col * SQUARE_PIXEL_SIZE, square.row * SQUARE_PIXEL_SIZE));
			miss.draw(*spriteRenderer);
		}
	}
	

	
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
	delete playerBoardRenderer;
	playerBoardRenderer = nullptr;
	delete radarBoardRenderer;
	radarBoardRenderer = nullptr;
	delete spriteRenderer;
	spriteRenderer = nullptr;
	
	glm::mat4 projection = glm::ortho<GLfloat>(0.0f, static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT), 0.0f, -1.0f, 1.0f);

	//Configure shaders
	ResourceManager::getShader("water_grid").use().setInt("image", 0);
	ResourceManager::getShader("water_grid").setMat4("projection", projection);
	ResourceManager::getShader("basic_sprite").use().setInt("image", 0);
	ResourceManager::getShader("basic_sprite").setMat4("projection", projection);
	ResourceManager::getShader("radar2").use().setInt("image", 0);
	ResourceManager::getShader("radar2").setMat4("projection", projection);



	playerBoardRenderer = new SpriteRenderer(ResourceManager::getShader("water_grid"));
    spriteRenderer = new SpriteRenderer(ResourceManager::getShader("basic_sprite"));
	radarBoardRenderer = new SpriteRenderer(ResourceManager::getShader("radar2"));

}


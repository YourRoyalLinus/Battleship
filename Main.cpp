#include <SDL.h>
#include <glad.h>
#include <SDL_image.h>
#include "Board.h"
#include "Player.h"
#include <algorithm>
#include <ctime>
#include "PlayerBoard.h"
#include "RadarBoard.h"
#include <SDL_ttf.h>
#include <SDL_opengl.h>
//TODO: LEAKING MEMORY RIGHT NOW FROM EVERYWHERE THAT USES A TEXTURE!!!!!! UH OH!!!! Implement destructors


const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const int SQUARE_PIXEL_SIZE = 75; //Magic numbers WOW!
const int SQUARE_PIXEL_HEIGHT = 75;

void handleInput();
void update();
void render();


//Shaders
const char* vertexShaderSource = "#version 460 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char* fragmentShaderSource = "#version 460 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";


//The window we'll be rendering to
SDL_Window* window = NULL;

//OpenGL context
SDL_GLContext gContext;

void init() {
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
	}
	SDL_GL_LoadLibrary(NULL);

	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 6 );

	window = SDL_CreateWindow( "BATTLESHIP", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if( window == NULL ){
		printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
	}
	
	//Create context
	gContext = SDL_GL_CreateContext( window );
	if( gContext == NULL ){
		printf( "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError() );
	}
	
	// Check OpenGL properties
	printf("OpenGL loaded\n");
	if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
		printf("Failed to initialize GLAD!");
		exit(-2);
	}
	//printf("Vendor:   %s\n", glGetString(GL_VENDOR));
	//printf("Renderer: %s\n", glGetString(GL_RENDERER));
	//printf("Version:  %s\n", glGetString(GL_VERSION));


	//Use Vsync
	if( SDL_GL_SetSwapInterval( 1 ) < 0 )
	{
		printf( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
	}

	// Disable depth test and face culling.
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glClearColor(0.0f, 0.5f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	SDL_GL_SwapWindow(window);


		
}

enum class GameState {
	SETUP,
	PLAYING,
	OVER
};

GameState state;
bool humanTurn = true;

void teardown() {
	
	IMG_Quit();
	SDL_Quit();
}


int main(int argc, char* argv[]) {

	//This is how I'm doing RNG for now!
	srand((unsigned)time(NULL));

	//Initialize SDL
	init();


	//Build and compile shader program
	//vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL /* <- Assume the source in NUL terminated */);
	glCompileShader(vertexShader);


	//Check for shader compiler errors.
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "VERTEX SHADER COMPILATION FAILED!:\n" << infoLog << std::endl;
	}

	//fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL /* <- Assume the source in NUL terminated */);
	glCompileShader(fragmentShader);

	//Check for shader compiler errors.
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "FRAGMENT SHADER COMPILATION FAILED!:\n" << infoLog << std::endl;
	}

	//Link Shaders
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//Check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "SHADER LINKING FAILED!:\n" << infoLog << std::endl;
	}
	//Don't need shader IDs anymore
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	//Set up vertex buffer and index buffer
	GLfloat vertices[] = {
		0.5f, 0.5f, 0.0f, //top right
		0.5f, -0.5f, 0.0f, //bottom right
		-0.5f, -0.5f, 0.0f, //bottom left
		-0.5f, 0.5f, 0.0f, //top left
	};
	GLuint indeces[] = {
		0, 1, 3, //first CW triangle
		1, 2, 3  //second CW triangle
	};

	//Create Vertex Array Object, Vertex Buffer Object, & Element Buffer Object. 
	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//Bind VAO
	glBindVertexArray(VAO);
	//Bind and configure buffers.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces), indeces, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Unbind buffers
	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 





	//TODO:!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! TEST CODE FOR GL STUFF !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	while (1) {
		update();
		
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		SDL_GL_SwapWindow(window);



	}

	
	//Make human player
	Player human;
	//Make Neural Net Artificial Intelligence Blockchain Smart BattleAI9000
	Player computer;
	//Make human player's board
	PlayerBoard humanBoard;
	//Computer board which looks like a radar board to the player
	RadarBoard computerBoard;

	//Texture chatBox("Textures\\BattleshipChat.png");


	state = GameState::SETUP;

	SDL_Event event; //input event
	

	while (state == GameState::SETUP) {

		if (humanTurn) {

			if (human.ships.empty()) {
				//state = GameState::PLAYING;
				humanTurn = false;
				continue;
			}

			Ship& currentShipRef = human.ships.back();

			int mouseXSquare, mouseYSquare; //what square is the mouse currently in.

			while (SDL_PollEvent(&event) != 0) {
				
				computerBoard.draw();
				humanBoard.draw();

				SDL_GetMouseState(&mouseXSquare, &mouseYSquare);
				/* Stupid hack!!! adjust the mouse by setup board position offset */
				//mouseXSquare -= Renderer::GUESS_BOARD_VIEWPORT.w + Renderer::CHAT_VIEWPORT.w;
				/* Clamp position to grid square size */
				//mouseXSquare /= Board::SQUARE_PIXEL_SIZE;
				mouseYSquare /= Board::SQUARE_PIXEL_SIZE;

				currentShipRef.snapToPosition({ mouseYSquare, mouseXSquare }); //Ypos = row, Xpos = col.
				currentShipRef.draw();


				if (event.type == SDL_QUIT) {
					exit(-1);
				}
								
				if (event.type == SDL_MOUSEBUTTONDOWN) {
					if (event.button.button == SDL_BUTTON_LEFT) { //button.button ???? OK
						if (humanBoard.placeShip(currentShipRef, currentShipRef.coords))
						{
							human.ships.pop_back();
							humanBoard.print();
						}
						else {
							
							printf("You can't place a ship there!\n"); 
							
						}
					}
					else if (event.button.button == SDL_BUTTON_RIGHT) {
						currentShipRef.rotate();
					}

				}

			}
		}

		//TODO: THIS IS GARBAGE. Make this more sophisticaed and there is some abstraction to be done.
		if (!humanTurn) {
			while (!computer.ships.empty()) {
				Ship& computerShipRef = computer.ships.back();
				int row = rand() % 8;
				int col = rand() % 8;
				
				computerShipRef.snapToPosition({ row, col });

				if (computerBoard.placeShip(computerShipRef, computerShipRef.coords)) {
					computer.ships.pop_back();
				}
			}
			humanBoard.print();
			//computerBoard.print();
			state = GameState::PLAYING;
			humanTurn = true;
		}

	}
	
	while (state == GameState::PLAYING) {

		if (humanTurn) {
			//Handle events on queue
			while (SDL_PollEvent(&event) != 0)
			{
				//User requests quit
				if (event.type == SDL_QUIT) //If you 'X' out
				{
					state = GameState::OVER;
				}

				if (event.type == SDL_MOUSEBUTTONDOWN) {

					if (event.button.button == SDL_BUTTON_LEFT) {
						int mouseXSquare, mouseYSquare; //what square is the mouse currently in.

						SDL_GetMouseState(&mouseXSquare, &mouseYSquare);

						/* Stupid hack!!! adjust the mouse by setup board position offset */
						//mouseXSquare -= Renderer::CHAT_VIEWPORT.w;

						/* Clamp position to grid square size */
						mouseXSquare /= Board::SQUARE_PIXEL_SIZE;
						mouseYSquare /= Board::SQUARE_PIXEL_SIZE;

						//std::cout << "You guessed ROW: " << mouseYSquare << "COL: " << mouseXSquare << std::endl;

						computerBoard.guess({ mouseYSquare, mouseXSquare }); //Again Y = ROW, X = COL!
					
						humanTurn = false;
					}
				}
			}

		}

		if (!humanTurn) {
			int row = rand() % 8;
			int col = rand() % 8;

			humanBoard.guess({ row, col });
		
			humanTurn = true;

		}
		


		
		computerBoard.draw();
		humanBoard.draw();
	

		if (humanBoard.activeShips.empty()) {
			state = GameState::OVER;
			std::cout << "GAME OVER! COMPUTER WON! YOU SUCK!" << std::endl;
		}
		else if (computerBoard.activeShips.empty()) {
			state = GameState::OVER;
			std::cout << "GAME OVER! YOU WON!" << std::endl;
		}
	}
	
	teardown();

	return 0;
	
}

void handleInput() {
	SDL_Event event; //input event
	if (SDL_PollEvent(&event) != 0) {
		if (event.type == SDL_QUIT) //If you 'X' out
		{
			state = GameState::OVER;
			exit(1);
		}
		if (event.type == SDL_WINDOWEVENT) {
			if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
				int width = event.window.data1;
				int height = event.window.data2;
				printf("width:%d height:%d", width, height);
				SDL_SetWindowSize(window, event.window.data1, event.window.data2);
				glViewport(0, 0, event.window.data1, event.window.data2);
			}
		}

		if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym) {
			case SDLK_q:
				printf("q!!!!\n");
				break;
			}
			
		}
	}
}

void update() {
	handleInput();
}

void render() {

}




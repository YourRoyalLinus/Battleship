#include "PlayerBoard.h"
#include "Renderer.h"


PlayerBoard::PlayerBoard() {
	gridImagePath = "Textures\\BattleshipGrid.png";

	missIconImagePath = "Textures\\BattleShip_Miss.png";

	hitIconImagePath = "Textures\\Battleship_Hit.png";

	gridTexture.loadFromFile(gridImagePath);
	missIconTexture.loadFromFile(missIconImagePath);
	hitIconTexture.loadFromFile(hitIconImagePath);
}

void PlayerBoard::draw() {
	Renderer::setViewPort(&Renderer::PLACE_BOARD_VIEWPORT);
	Board::draw();
	for (auto ship : activeShips)
		ship.draw();
	//TODO: Dumb hack! I'm doing this because the ship sprites will draw over the hit markers right now
	for (auto square : guessedSquares) {
		SDL_Rect dest = { square.col * 75, square.row * 75 , 75, 75 }; //AGAIN Y = ROW, X = COL. I KEEP WRITING THIS BECAUSE IT IS VERY COUNTER-INTUITIVE TO ME!!!!
		if (square.occupied) {
			Renderer::render(hitIconTexture, 0, &dest);
		}
		else {
			Renderer::render(missIconTexture, 0, &dest);
		}
	}
}
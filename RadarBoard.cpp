#include "RadarBoard.h"
#include "Renderer.h"


RadarBoard::RadarBoard() {
	gridImagePath = "Textures\\BattleshipGuessBoard.png";

	missIconImagePath = "Textures\\BattleShip_Radar_Miss.png";

	hitIconImagePath = "Textures\\Battleship_Radar_Hit.png";

	gridTexture.loadFromFile(gridImagePath);
	missIconTexture.loadFromFile(missIconImagePath);
	hitIconTexture.loadFromFile(hitIconImagePath);

}

void RadarBoard::draw() {
	Renderer::setViewPort(&Renderer::GUESS_BOARD_VIEWPORT);
	Board::draw();
}
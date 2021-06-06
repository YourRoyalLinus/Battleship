#include "RepositionShipCommand.h"
#include "InputHandler.h"
#include "Board.h"

void RepositionShipCommand::execute() {
	//This is the board square that the mouse is currently on.
	int currentSquareX = (InputHandler::mouseX - 600) / Board::SQUARE_PIXEL_SIZE;
	int currentSquareY = InputHandler::mouseY / Board::SQUARE_PIXEL_SIZE;

	this->player.shipToPlace->snapToPosition({ currentSquareY, currentSquareX });

}
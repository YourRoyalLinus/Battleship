#include "HumanGuessStrategy.h"
#include "InputHandler.h"
#include "Board.h"

void HumanGuessStrategy::guess(Player& player, Player& opponent) {
	int mouseYSquare = (InputHandler::mouseX) / Board::SQUARE_PIXEL_SIZE; //Vector<Vector> -> XY-Plane rotated 90 deg clockwise -> horizontal movement is on the Y axis
	int mouseXSquare = (InputHandler::mouseY) / Board::SQUARE_PIXEL_SIZE; //Vector<Vector> -> XY-Plane rotated 90 deg clockwise -> vertical movement is on the X axis
	opponent.board->guess({ mouseXSquare, mouseYSquare });
}

#include "SinglePlayerGuess.h"
#include "InputHandler.h"
#include "Board.h"
#include "Player.h"

void SinglePlayerGuess::guess(Player& player, Player& opponent) {
	int row = (InputHandler::mouseY) / Board::SQUARE_PIXEL_SIZE; //Vector<Vector> -> XY-Plane rotated 90 deg clockwise -> vertical movement is on the X axis
	int col = (InputHandler::mouseX) / Board::SQUARE_PIXEL_SIZE; //Vector<Vector> -> XY-Plane rotated 90 deg clockwise -> horizontal movement is on the Y axis

	opponent.board->guess({ row, col });
}

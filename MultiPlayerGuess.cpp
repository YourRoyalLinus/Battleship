#include "MultiPlayerGuess.h"
#include "InputHandler.h"
#include "Board.h"

void MultiPlayerGuess::guess(Player& player, Player& opponent) {
	int row = (InputHandler::mouseY) / Board::SQUARE_PIXEL_SIZE; //Vector<Vector> -> XY-Plane rotated 90 deg clockwise -> vertical movement is on the X axis
	int col = (InputHandler::mouseX) / Board::SQUARE_PIXEL_SIZE; //Vector<Vector> -> XY-Plane rotated 90 deg clockwise -> horizontal movement is on the Y axis

	int resp = net->SendNextGuess(row, col);
	//Unable to send data to the other player before timeout period expired(60 seconds)
	if (resp == 0) {
		std::cout << "Lost connection to peer...\n";
		std::cout << "Game Over!\n";
		//TODO: add game over state
		exit(-1);
		return;
	}


}

#include "ComputerShipPlacement.h"
#include "Player.h"
void ComputerShipPlacement::place()
{
	do {
		//If placing failed generate new coords & try again
		int row = rand() % 8;
		int col = rand() % 8;
		player.shipToPlace->snapToPosition({ row, col });
	} while (!player.board->placeShip(*player.shipToPlace, player.shipToPlace->coords));
	player.ships.pop_back();
	if (!player.ships.empty())
		player.shipToPlace = &player.ships.back();
	player.board->print();
}

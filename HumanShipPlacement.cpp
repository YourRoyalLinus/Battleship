#include "HumanShipPlacement.h"
#include "Player.h"
void HumanShipPlacement::place() {
	if (player.board->placeShip(*player.shipToPlace, player.shipToPlace->coords)) {
		player.ships.pop_back();
		if (!player.ships.empty())
			player.shipToPlace = &player.ships.back();
		player.board->print();
		std::cout << "\n\n" << std::endl;
	}
}

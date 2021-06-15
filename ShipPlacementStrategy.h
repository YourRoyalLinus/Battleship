#pragma once
class Player;
class ShipPlacementStrategy
{
public:
	ShipPlacementStrategy(Player& player) : player(player) {};
	virtual void place() = 0;
protected:
	Player& player;
};


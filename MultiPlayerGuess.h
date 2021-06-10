#pragma once
#include "GuessStrategy.h"
#include "PeerNetwork.h"
class MultiPlayerGuess : public GuessStrategy
{
public:
	MultiPlayerGuess(PeerNetwork* net) : net(net) { /*...*/ }
	void guess(Player& player, Player& opponent) override;
private:
	PeerNetwork* net;
};


#pragma once
#include "Peer.h"

class ConnectingPeer : public Peer
{
	public:
		ConnectingPeer();
		SOCKET SearchForOpponent(std::string networkIp);
	private:
		void ConnectToSocket();
};




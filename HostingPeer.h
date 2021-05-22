#pragma once
#include "Peer.h"

class HostingPeer : public Peer
{
	public:
		HostingPeer();
		SOCKET ListenForOpponent();
	private:
		void BindSocket();
		void Listen();
		void AcceptConnection();
};

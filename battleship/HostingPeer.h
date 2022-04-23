#pragma once
#include "Peer.h"

class HostingPeer : public Peer
{
	public:
		HostingPeer();
		void ListenForOpponent();
		SOCKET AcceptConnection();
	private:
		void BindSocket();
		void Listen();
};

#pragma once
#include "Peer.h"

class PeerNetwork
{
	public:
		PeerNetwork();
		~PeerNetwork();

		void ConnectToRandomOpponent(std::string networkStartIP);
		void ConnectToFriend(std::string gameToken);
		//int RegisterPeer(Peer* peer); TODO Build Peer Routing Table
		int RemovePeer(Peer* peer); //TODO Extend to remove Peer from Routing Table

		int SendTurn(bool turn);
		int SendNextGuess(int x, int y);
		int SendGuessResult(int oppGuessX, int oppGuessY, bool res);
		int SendGameOver();
		Payload::Buffer ReceiveData();

		Peer::PeerType peerType;
	private:
		Peer* playerPeer = nullptr;
		SOCKET gameSocket = INVALID_SOCKET;
};


#pragma once
#include "Peer.h"
#include "HostingPeer.h"
#include "ConnectingPeer.h"

class PeerNetwork
{
	public:
		PeerNetwork();
		~PeerNetwork();

		//bool ConnectToRandomOpponent(std::string networkStartIP);
		//void ConnectToFriend(std::string gameToken);
		//int RegisterPeer(Peer* peer); TODO Build Peer Routing Table
		//int RemovePeer(Peer* peer); //TODO Extend to remove Peer from Routing Table

		bool searchForOpponent(std::string networkStartIP);
		void hostGame();
		bool findOpponent();

		int SendTurn(bool turn);
		int SendNextGuess(int x, int y);
		int SendGuessResult(int oppGuessX, int oppGuessY, bool res);
		int SendGameOver();
		Payload::Buffer ReceiveData();

		Peer::PeerType peerType;
	private:
		Peer* playerPeer = nullptr;
		SOCKET gameSocket = INVALID_SOCKET;

		//temp
		HostingPeer* hPeer = nullptr;
		ConnectingPeer* cPeer = nullptr;
};


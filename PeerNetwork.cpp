#pragma once
#include "PeerNetwork.h"
#include "ConnectingPeer.h"
#include "HostingPeer.h"

PeerNetwork::PeerNetwork() {
}

PeerNetwork::~PeerNetwork() {
	delete playerPeer;
	gameSocket = INVALID_SOCKET;
}

/*
*/
void PeerNetwork::ConnectToRandomOpponent(std::string networkStartIP) {
	HostingPeer* hPeer = new HostingPeer();
	ConnectingPeer* cPeer = new ConnectingPeer();

	SOCKET listeningSocket = INVALID_SOCKET;
	SOCKET connectingSocket = INVALID_SOCKET;

	connectingSocket = cPeer->SearchForOpponent(networkStartIP); //TESTING
	if (connectingSocket == INVALID_SOCKET) {
		RemovePeer(cPeer);
		listeningSocket = hPeer->ListenForOpponent();
		if (listeningSocket == INVALID_SOCKET) {
			RemovePeer(hPeer);
			std::cout << "No available opponents are on the network. Please try again later.\n";
			exit(0);
		}
		else {
			playerPeer = hPeer;
			gameSocket = listeningSocket;
			peerType = Peer::PeerType::HOSTING_PEER;
			std::cout << "CONNECTED TO PLAYER SEARCHING FOR OPPONENET!\n";
		}

	}
	else {
		playerPeer = cPeer;
		gameSocket = connectingSocket;
		peerType = Peer::PeerType::CONNECTING_PEER;
		std::cout << "CONNECTED TO OPPONENT IN QUEUE!\n";
	}
}

/*
	Connect to a specific player
	Each player will have a game token created that will be a some sort of hash of their IP
	Sharing your token will allow another person to request a game
*/
void PeerNetwork::ConnectToFriend(std::string gameToken) {
	//TODO
}

/*
	GC Peer pointer
	In the future, will remove peer from the Peer Routing Table as well
*/
int PeerNetwork::RemovePeer(Peer* peer) {
	if (peer != nullptr) {
		delete peer;
		return 0;
	}

	return -1;
}

int PeerNetwork::SendTurn(bool turn) {
	Payload::Buffer buffer;
	buffer.bufferType = Payload::BufferType::TURN;
	buffer.yourTurn = !turn;
	return playerPeer->SendGuess(buffer);
}
int PeerNetwork::SendNextGuess(int x, int y) {
	Payload::Buffer buffer;
	buffer.bufferType = Payload::BufferType::NEXT_GUESS;
	buffer.xChoordGuess = x;
	buffer.yChoordGuess = y;
	return playerPeer->SendGuess(buffer);
}

int PeerNetwork::SendGuessResult(int oppGuessX, int oppGuessY, bool res) {
	Payload::Buffer buffer;
	buffer.bufferType = Payload::BufferType::GUESS_RESULT;
	buffer.xChoordGuess = oppGuessX;
	buffer.yChoordGuess = oppGuessY;
	buffer.prevGuessHit = res;
	return playerPeer->SendGuess(buffer);
}

int PeerNetwork::SendGameOver() {
	Payload::Buffer buffer;
	buffer.bufferType = Payload::BufferType::GAME_OVER;
	return playerPeer->SendGuess(buffer);
}

Payload::Buffer PeerNetwork::ReceiveData() {
	return playerPeer->ReceiveData();
};